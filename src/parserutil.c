#include "parserutil.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memwrapper.h"
#include "parser.h"
#include "ptrstack.h"
#include "parsergraph.h"

parseTreeNode *parseTreePrune(parseTreeNode *root){

	return root;
}


parseTreeNode *parseTreeDeallocateEpsilon(parseTreeNode *root){
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

		if(next->type == EPSILON){
			// Backtrack to find branch root
			parseTreeNode *broot = next->parent;

			while(broot->parent != NULL){
				if(broot->parent->childCount != 1){
					// This is the one
					break;
				}

				// Update next
				broot = broot->parent;
			}

			// Destroy branch
			bool updateRoot = broot->parent == root;
			parseTreeNode *retPtr = parseTreeDeallocateBranch(broot);
			if(updateRoot){
				root = retPtr;
			}
			//parserGenerateDotFile(root, "pruningTree.dot");
			//printf("[PARSE] Generated tree view... [press enter]");
			//getchar();
		}
	}

	// Destroy stack used to track child nodes
	stackDestroy(top);
	// Free stack root
	mwfree(topElement, sizeof(pNode));

	return root;
}
