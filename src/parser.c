#include <lexer.h>
#include <memwrapper.h>
#include <parser.h>
#include <ptrstack.h>
#include <stdio.h>
#include <string.h>
#include <switches.h>

static parseTreeNode *root = NULL;
static bool parserStateOk = true;

void parserInit() {
	parseTreeCreateRoot();
}

void parseTreeCreateRoot() {
	// Create the root node of the parse tree
	root = (parseTreeNode *) mwalloc(sizeof(parseTreeNode));
	root->parent = NULL;
	root->type = SYM_S;
	root->value = NULL;
	root->rule = NULL;
	root->childCount = 0;
}

void parseTreeCreateNode(parseTreeNode *parent, int index, tokenType type) {
	if (index >= parent->childCount) {
		// TODO: dbg msg
		parserStateOk = false;
		return;
	}

	if (parent->children[index] != NULL) {
		// TODO: dbg msg
		parserStateOk = false;
		return;
	}

	// Allocate memory for the child and set default values
	parseTreeNode *c = parent->children[index] = (parseTreeNode *) mwalloc(
			sizeof(parseTreeNode));
	c->parent = parent;
	c->type = type;
	c->value = NULL;
	c->rule = NULL;
	c->childCount = 0;
}

parseTreeNode *parseTreeApplyRule(parseTreeNode *node, parserRule *rule) {
	node->rule = rule;

	int sz = 0;
	for (int i = 0; i < EXPR_N; i++) {
		if (rule->expr[i] == -1)
			break;
		sz++;
	}

	if (sz == 0) {
		// TODO: dbg msg
		parserStateOk = false;
		return node;
	}

	// Realloc node
	node = parseTreeNodeRealloc(node, sz);

	if (node == NULL) {
		// TODO: dbg msg
		parserStateOk = false;
		return node;
	}

	// Update child count
	node->childCount = sz;

	for (int i = 0; i < sz; i++) {
		// Create child nodes
		parseTreeCreateNode(node, i, rule->expr[i]);
	}

	return node;
}

parseTreeNode *parseTreeNodeRealloc(parseTreeNode *node, int newChildCount) {
	// Saves the reference to the parent node
	parseTreeNode *parent = node->parent;
	int parIndex = -1;

	if (newChildCount <= node->childCount) {
		// TODO: dbg msg
		parserStateOk = false;
		return NULL;
	}

	// Get the position where the node is reference in the parent
	if (parent != NULL) {
		// Root node, no reference to update
		for (int i = 0; i < parent->childCount; i++) {
			if (parent->children[i] == node) {
				parIndex = i;
				break;
			}
		}

		if (parIndex == -1) {
			// TODO: dbg msg
			parserStateOk = false;
			return NULL;
		}
	}

	int oldChildCount = node->childCount;

	// Reallocate memory
	parseTreeNode *ptr = (parseTreeNode *) mwrealloc(node,
			sizeof(parseTreeNode) + sizeof(void *) * node->childCount,
			sizeof(parseTreeNode) + sizeof(void *) * newChildCount);

	if (parent != NULL) {
		// Update parent reference
		parent->children[parIndex] = ptr;
	}

	// Update children
	for (int i = 0; i < oldChildCount; i++) {
		if (ptr->children[i] != NULL) {
			ptr->children[i]->parent = ptr;
		}
	}

	// Set new pointers to NULL
	memset(&ptr->children[oldChildCount], 0,
			sizeof(void *) * (newChildCount - oldChildCount));

	// Update child count
	ptr->childCount = newChildCount;

	// Return new pointer to the node
	return ptr;
}

void parseTreeCopyValue(parseTreeNode *node, char* str) {
	node->value = str;
	// Should not create another copy since generated strings will be
	// deallocated at the end of the program.
}

bool parserOk() {
	return parserStateOk;
}

void parseTreeDestroy() {
	// Creates a stack to store references to child nodes.
	pNode *topElement = stackCreateNode();
	pNode **top = &topElement;
	// Push the root node to the stack
	stackPush(top, root);

	// While the stack is not empty
	while (topElement->p != NULL) {
		// Pop stack
		parseTreeNode *next = (parseTreeNode *) topElement->p;
		stackPop(top);

		// Add children to the stack
		for (int i = 0; i < next->childCount && next->children != NULL; i++) {
			if (next->children[i] != NULL) {
				stackPush(top, next->children[i]);
			}
		}

		// Free memory
		mwfree(next,
				sizeof(parseTreeNode)
						+ sizeof(parseTreeNode *) * next->childCount);
	}

	// Destroy stack used to track child nodes
	stackDestroy(top);
	// Free stack root
	mwfree(topElement, sizeof(pNode));
}

parseTreeNode *parserExec() {
	// Create the parser stack, this will store the symbol that is expected from
	// the input.
	pNode *topElement = stackCreateNode();
	pNode **top = &topElement;

	// Push start symbol
	stackPush(top, NULL); // End of input (CHAR_NUL)
	stackPush(top, root);

	// While the parser stack is not empty
	while (!stackEmpty(top)) {
		parseTreeNode *t = (parseTreeNode *) topElement->p;
		token nextToken = lexerPeek();

		if (t == NULL) {
			printf("[PARSE] At end of stack\n");
			stackPop(top);
			break;
		}

		// Pop stack
		stackPop(top);

		// If the expected token is EPSILON (empty string), continue.
		if (t->type == EPSILON) {
			// Read nothing
			if (PAUSE_PARSER_ON_STEP) {
				printf("[PARSE] Read token EPSILON\n");
				stackDump(top);
				printf("[PARSE] Next token: %s ",
						tokenTypeName[nextToken.type + 1]);
				if (nextToken.value != NULL) {
					printf("value = %s\n", nextToken.value);
				} else {
					printf("\n");
				}
				if (t != NULL) {
					printf("[PARSE] Expected token: %s\n",
							tokenTypeName[t->type + 1]);
				} else {
					printf("[PARSE] Expected token: NULL\n");
				}
				memstat();
				printf("[PARSE] [press enter]");
				getchar();
			}
		} else {
			// Otherwise check if the next expected token is a terminal.
			if (isTerminal(t->type)) {
				// If t is terminal or CHAR_NUL and the next token in the input
				// matches, then consume the token.
				if (t->type == nextToken.type) {
					// Read token
					nextToken = lexerNext();
					t->value = nextToken.value;

					if (PAUSE_PARSER_ON_STEP) {
						printf("[PARSE] Read terminal token\n");
					}
				} else {
					// Unexpected token
					// TODO: output error
					parserStateOk = false;

					if (PAUSE_PARSER_ON_STEP) {
						printf("[PARSE] Parser error: unexpected token\n");
						stackDump(top);
						printf("[PARSE] Next token: %s ",
								tokenTypeName[nextToken.type + 1]);
						if (nextToken.value != NULL) {
							printf("value = %s\n", nextToken.value);
						} else {
							printf("\n");
						}
						if (t != NULL) {
							printf("[PARSE] Expected token: %s\n",
									tokenTypeName[t->type + 1]);
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
				// Then check whether any rules can be applied to the
				// non-terminal based on the next token.
				int rule = parserPredictTable[t->type][nextToken.type];

				if (rule == -1) {
					// Syntax error if no rules match (table query yields -1)
					// TODO: output error
					parserStateOk = false;

					if (PAUSE_PARSER_ON_STEP) {
						printf("[PARSE] Parser error: syntax error\n");
						stackDump(top);
						printf("[PARSE] Next token: %s ",
								tokenTypeName[nextToken.type + 1]);
						if (nextToken.value != NULL) {
							printf("value = %s\n", nextToken.value);
						} else {
							printf("\n");
						}
						if (t != NULL) {
							printf("[PARSE] Expected token: %s\n",
									tokenTypeName[t->type + 1]);
						} else {
							printf("[PARSE] Expected token: NULL\n");
						}
						memstat();
						printf("[PARSE] [press enter]");
						getchar();
					}

					break;
				} else {
					// Apply rule if a rule matches
					bool isRoot = t == root;
					t = parseTreeApplyRule(t, &parserRules[rule]);

					if (isRoot) {
						// Destroy root
						root = t;
					}

					// Push the terminals or non-terminals in order on the stack
					for (int i = t->childCount - 1; i >= 0; i--) {
						if (t->children[i] == NULL) {
							// Unexpected state
							parserStateOk = false;
							break;
						}

						if (t->children[i]->type == -1) {
							// Unexpected state
							parserStateOk = false;
							break;
						}

						stackPush(top, t->children[i]);
					}

					// Exit if an error occured
					if (!parserStateOk) {
						break;
					}

					if (PAUSE_PARSER_ON_STEP) {
						printf("[PARSE] Parser matched rule\n");
						parserDisplayRule(rule);
						stackDump(top);
						printf("[PARSE] Next token: %s ",
								tokenTypeName[nextToken.type + 1]);
						if (nextToken.value != NULL) {
							printf("value = %s\n", nextToken.value);
						} else {
							printf("\n");
						}
						if (t != NULL) {
							printf("[PARSE] Expected token: %s\n",
									tokenTypeName[t->type + 1]);
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

	if (PAUSE_PARSER_ON_STEP) {
		printf("[PARSE] Parser loop ended\n");
		stackDump(top);
		memstat();
	}

	// The parser has reached the bottom of the stack, entered an unexpected
	// state or reached the end of input.
	token nt = lexerPeek();
	if (nt.type == CHAR_NUL && parserStateOk && stackEmpty(top)) {
		// ACCEPT if the parser has reached the bottom of the stack and reached
		// the end of the input.
		if (PAUSE_PARSER_ON_STEP) {
			printf("[PARSE] Input accepted\n");
		}
	} else {
		// REJECT otherwise
		if (PAUSE_PARSER_ON_STEP) {
			printf("[PARSE] Input rejected\n");
		}
		parserStateOk = false;
	}

	if (PAUSE_PARSER_ON_STEP) {
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
