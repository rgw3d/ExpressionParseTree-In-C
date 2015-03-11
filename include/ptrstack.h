#pragma once

#include <stdbool.h>

/**
 * A node in a generic stack structure.
 */
typedef struct pNode {
	void *p;			///< a pointer to a memory location.
	struct pNode *prev; ///< a pointer to the previous node in the stack.
} pNode;

/**
 * Tests whether the stack is empty.
 *
 * @param top a pointer to the pointer to the top element of the stack.
 * @return true if the stack is empty, false otherwise.
 */
bool stackEmpty(pNode **top);
/**
 * Destroys the entire stack. The top element if left untouched, it will have
 * to be deallocated manually with mwfree.
 *
 * @param top a pointer to the pointer to the top element of the stack.
 */
void stackDestroy(pNode **top);
/**
 * Pushes a pointer on to the stack.
 *
 * @param top a pointer to the pointer to the top element of the stack. The
 * 			  pointer referenced by this pointer will be automatically updated
 * 			  to point to the new top element of the stack.
 * @param p the pointer to push on the stack.
 */
void stackPush(pNode **top, void *p);
/**
 * Pops the top node from the stack.
 *
 * @param top a pointer to the pointer to the top element of the stack. The
 * 			  pointer referenced by this pointer will be automatically update
 * 			  to point to the new top element of the stack.
 */
void stackPop(pNode **top);
/**
 * Creates an empty stack node to be used as a root for a stack.
 *
 * @return a pointer to the created node.
 */
pNode *stackCreateNode();

/**
 * Prints the content of the stack.
 *
 * @param top a pointer to the pointer to the top element of the stack.
 */
void stackDump(pNode **top);
