#include "parsergraph.h"
#include <stdio.h>
#include "parser.h"
#include "token.h"
#include "ptrstack.h"
#include "memwrapper.h"
#include <inttypes.h>

void parserGenerateDotFile(parseTreeNode *root) {
	// Creates a stack to store references to child nodes.
	pNode *topElement = stackCreateNode();
	pNode **top = &topElement;
	// Push the root node to the stack
	stackPush(top, root);

	// Open file
	FILE *output = fopen("parsetree.dot", "w");

	fprintf(output, "digraph parseTree{\n");

	// While the stack is not empty
	while (topElement->p != NULL) {
		// Pop stack
		parseTreeNode *next = (parseTreeNode *) topElement->p;
		stackPop(top);

		// Add node styles
		fprintf(output, "  \"%016lX\" ", (uintptr_t)next);
		if(next->value == NULL){
			fprintf(output, "[shape=record label=\"{%s|0x%016lX}\" ", tokenTypeName[next->type + 1], (uintptr_t)next);
		} else {
			fprintf(output, "[shape=record label=\"{%s|0x%016lX}|%s\" ", tokenTypeName[next->type + 1], (uintptr_t)next, next->value);
		}

		if(next->type == EPSILON){
			fprintf(output, "fillcolor=lightgrey style=filled ");
		}

		if(next->type == CHAR_NUL){
			fprintf(output, "fillcolor=lightred style=filled ");
		}

		if(isTerminalStrict(next->type)){
			fprintf(output, "fillcolor=lightblue style=filled ");
		}

		fprintf(output, "];\n");

		// Add children to the stack
		for (int i = 0; i < next->childCount && next->children != NULL; i++) {
			if (next->children[i] != NULL) {
				stackPush(top, next->children[i]);

				// Add graph connection
				fprintf(output, "  \"%016lX\" -> \"%016lX\" ", (uintptr_t)next, (uintptr_t)next->children[i]);

				// Add connection styles
				if(next->children[i]->type == EPSILON){
					fprintf(output, "[style=dashed color=red]");
				} else if(next->children[i]->type == CHAR_NUL){
					fprintf(output, "[style=dashed color=green]");
				} else if(isTerminalStrict(next->children[i]->type)){
					fprintf(output, "[style=dashed color=blue]");
				} else {
					fprintf(output, "[dir=none]");
				}

				fprintf(output, ";\n");
			}
		}
	}

	fprintf(output, "}\n");
	fclose(output);

	// Destroy stack used to track child nodes
	stackDestroy(top);
	mwfree(*top, sizeof(pNode));
}
