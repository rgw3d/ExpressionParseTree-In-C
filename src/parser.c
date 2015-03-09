#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memwrapper.h"
#include "parsergen.h"
#include "ptrstack.h"
#include "lexer.h"

static parseTreeNode *root = NULL;
static bool parserStateOk = true;

void parserInit(){
	parseTreeCreateRoot();
}

void parseTreeCreateRoot(){
	root = (parseTreeNode *)mwalloc(sizeof(parseTreeNode));
	root->parent = NULL;
	root->type = SYM_S;
	root->value = NULL;
	root->rule = NULL;
	root->childCount = 0;
}

void parseTreeCreateNode(parseTreeNode *parent, int index, tokenType type){
	if(index >= parent->childCount){
		// TODO: dbg msg
		parserStateOk = false;
		return;
	}

	if(parent->children[index] != NULL){
		// TODO: dbg msg
		parserStateOk = false;
		return;
	}

	parseTreeNode *c = parent->children[index] = (parseTreeNode *)mwalloc(sizeof(parseTreeNode));
	c->parent = parent;
	c->type = type;
	c->value = NULL;
	c->rule = NULL;
	c->childCount = 0;
}

parseTreeNode *parseTreeApplyRule(parseTreeNode *node, parserRule *rule){
	node->rule = rule;

	int sz = 0;
	for(int i = 0; i < EXPR_N; i++){
		if(rule->expr[i] == -1) break;
		sz++;
	}

	if(sz == 0){
		// TODO: dbg msg
		parserStateOk = false;
		return node;
	}

	// Realloc node
	node = parseTreeNodeRealloc(node, sz);

	if(node == NULL){
		// TODO: dbg msg
		parserStateOk = false;
		return node;
	}

	for(int i = 0; i < sz; i++){
		parseTreeCreateNode(node, i, rule->expr[i]);
	}

	// Update child count
	node->childCount = sz;
	return node;
}

parseTreeNode *parseTreeNodeRealloc(parseTreeNode *node, int newChildCount){
	parseTreeNode *parent = node->parent;
	int parIndex = -1;

	if(newChildCount <= node->childCount){
		// TODO: dbg msg
		parserStateOk = false;
		return NULL;
	}

	if(parent != NULL){
		// Root node, no reference to update
		for(int i = 0; i < parent->childCount; i++){
			if(parent->children[i] == node){
				parIndex = i;
				break;
			}
		}

		if(parIndex == -1){
			// TODO: dbg msg
			parserStateOk = false;
			return NULL;
		}
	}

	int oldChildCount = node->childCount;

	parseTreeNode *ptr = (parseTreeNode *)mwrealloc(node, sizeof(parseTreeNode) + sizeof(void *) * node->childCount, sizeof(parseTreeNode) + sizeof(void *) * newChildCount);

	if(parent != NULL){
		// Update parent reference
		parent->children[parIndex] = ptr;
	}

	// Update children
	for(int i = 0; i < oldChildCount; i++){
		if(ptr->children[i] != NULL){
			ptr->children[i]->parent = ptr;
		}
	}

	// Set new pointers to NULL
	memset(&ptr->children[oldChildCount], 0, sizeof(void *) * (newChildCount - oldChildCount));

	// Update child count
	ptr->childCount = newChildCount;

	return ptr;
}

void parseTreeCopyValue(parseTreeNode *node, char* str){
	node->value = str; // Too lazy to create another copy
}

bool parserOk(){
	return parserStateOk;
}

void parseTreeDestroy(){
	pNode *topElement = stackCreateNode();
	pNode **top = &topElement;
	stackPush(top, root);

	while(topElement->p != NULL){
		// Pop stack
		parseTreeNode *next = (parseTreeNode *)topElement->p;
		stackPop(top);

		// Add children
		for(int i = 0; i < next->childCount && next->children != NULL; i++){
			if(next->children[i] != NULL){
				stackPush(top, next->children[i]);
			}
		}

		// Free memory
		mwfree(next, sizeof(parseTreeNode) + sizeof(parseTreeNode *) * next->childCount);
	}

	// Destroy stack
	stackDestroy(top);
	mwfree(topElement, sizeof(pNode));
}


parseTreeNode *parserExec(){
	pNode *topElement = stackCreateNode();
	pNode **top = &topElement;

	// Push start symbol
	stackPush(top, NULL); // End of input (CHAR_NUL)
	stackPush(top, root);

	while(!stackEmpty(top)){
		parseTreeNode *t = (parseTreeNode *)topElement->p;
		token nextToken = lexerPeek();

		if(t == NULL){
			printf("[PARSE] At end of stack\n");
			stackPop(top);
			break;
		}

		if(PAUSE_PARSER_ON_STEP){
			printf("[PARSE] Parser step\n");
			stackDump(top);
			printf("[PARSE] Next token: %s ", tokenTypeName[nextToken.type + 1]);
			if(nextToken.value != NULL){
				printf("value = %s\n", nextToken.value);
			} else {
				printf("\n");
			}
			if(t != NULL){
				printf("[PARSE] Expected token: %s\n", tokenTypeName[t->type + 1]);
			} else {
				printf("[PARSE] Expected token: NULL\n");
			}
			memstat();
			printf("[PARSE] [press enter]");
			getchar();
		}

		// Pop stack
		stackPop(top);

		if(t->type == EPSILON){
			// Read nothing
			if(PAUSE_PARSER_ON_STEP){
				printf("[PARSE] Read token EPSILON\n");
				stackDump(top);
				printf("[PARSE] Next token: %s ", tokenTypeName[nextToken.type + 1]);
				if(nextToken.value != NULL){
					printf("value = %s\n", nextToken.value);
				} else {
					printf("\n");
				}
				if(t != NULL){
					printf("[PARSE] Expected token: %s\n", tokenTypeName[t->type + 1]);
				} else {
					printf("[PARSE] Expected token: NULL\n");
				}
				memstat();
				printf("[PARSE] [press enter]");
				getchar();
			}
		} else {
			if(isTerminal(t->type)){
				// If t is terminal or CHAR_NUL
				if(t->type == nextToken.type){
					// Read token
					nextToken = lexerNext();
					t->value = nextToken.value;

					if(PAUSE_PARSER_ON_STEP){
						printf("[PARSE] Read terminal token\n");
					}
				} else {
					// Unexpected token
					// TODO: output error
					parserStateOk = false;

					if(PAUSE_PARSER_ON_STEP){
						printf("[PARSE] Parser error: unexpected token\n");
						stackDump(top);
						printf("[PARSE] Next token: %s ", tokenTypeName[nextToken.type + 1]);
						if(nextToken.value != NULL){
							printf("value = %s\n", nextToken.value);
						} else {
							printf("\n");
						}
						if(t != NULL){
							printf("[PARSE] Expected token: %s\n", tokenTypeName[t->type + 1]);
						} else {
							printf("[PARSE] Expected token: NULL\n");
						}
						memstat();
						printf("[PARSE] [press enter]");
						getchar();
					}

					break;
				}
			} else {
				// If t is non-terminal
				int rule = parserPredictTable[t->type][nextToken.type];

				if(rule == -1){
					// Syntax error
					// TODO: output error
					parserStateOk = false;

					if(PAUSE_PARSER_ON_STEP){
						printf("[PARSE] Parser error: syntax error\n");
						stackDump(top);
						printf("[PARSE] Next token: %s ", tokenTypeName[nextToken.type + 1]);
						if(nextToken.value != NULL){
							printf("value = %s\n", nextToken.value);
						} else {
							printf("\n");
						}
						if(t != NULL){
							printf("[PARSE] Expected token: %s\n", tokenTypeName[t->type + 1]);
						} else {
							printf("[PARSE] Expected token: NULL\n");
						}
						memstat();
						printf("[PARSE] [press enter]");
						getchar();
					}

					break;
				} else {
					// Apply rule
					bool isRoot = t == root;
					t = parseTreeApplyRule(t, &parserRules[rule]);

					if(isRoot){
						// Destroy root
						root = t;
					}

					// Push nodes in order
					for(int i = t->childCount - 1; i >= 0; i--){
						if(t->children[i] == NULL){
							// Unexpected state
							parserStateOk = false;
							break;
						}

						if(t->children[i]->type == -1){
							// Unexpected state
							parserStateOk = false;
							break;
						}

						stackPush(top, t->children[i]);
					}

					if(!parserStateOk){
						break;
					}

					if(PAUSE_PARSER_ON_STEP){
						printf("[PARSE] Parser matched rule\n");
						parserDisplayRule(rule);
						stackDump(top);
						printf("[PARSE] Next token: %s ", tokenTypeName[nextToken.type + 1]);
						if(nextToken.value != NULL){
							printf("value = %s\n", nextToken.value);
						} else {
							printf("\n");
						}
						if(t != NULL){
							printf("[PARSE] Expected token: %s\n", tokenTypeName[t->type + 1]);
						} else {
							printf("[PARSE] Expected token: NULL\n");
						}
						memstat();
						printf("[PARSE] [press enter]");
						getchar();
					}
				}
			}
		}
	}

	if(PAUSE_PARSER_ON_STEP){
		printf("[PARSE] Parser loop ended\n");
		stackDump(top);
		memstat();
	}

	token nt = lexerPeek();
	if(nt.type == CHAR_NUL && parserStateOk && stackEmpty(top)){
		// ACCEPT
		if(PAUSE_PARSER_ON_STEP){
			printf("[PARSE] Input accepted\n");
		}
	} else {
		// REJECT
		if(PAUSE_PARSER_ON_STEP){
			printf("[PARSE] Input rejected\n");
		}
		parserStateOk = false;
	}

	if(PAUSE_PARSER_ON_STEP){
		memstat();
		printf("[PARSE] Parse complete [press enter]");
		getchar();
	}

	// Cleanup
	stackDestroy(top);

	// Destroy root
	mwfree(topElement, sizeof(pNode));

	return root;
}

// Copies the value of a node to another, this function ignores the children array and children count.
void parseTreeNodeCopy(parseTreeNode *from, parseTreeNode *to){
	to->parent = from->parent;
	to->type = from->type;
	to->value = from->value;
	to->rule = from->rule;
	to->childCount = 0;
}
