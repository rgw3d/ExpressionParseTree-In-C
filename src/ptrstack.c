#include <memwrapper.h>
#include <ptrstack.h>
#include <stdio.h>
#include <switches.h>

// Destroy an entire stack
// Take extra care when using this function as it does not deallocate the pointers stored in
// the nodes. Once the nodes are destroyed, the references are gone forever!
void stackDestroy(pNode **top) {
	while (*top != NULL && (*top)->prev != NULL) {
		// while the pointer to the top element is not null and the top element
		// is not the root. Pop the stack.
		stackPop(top);
	}
}

// Pushes a new pointer onto the stack and returns the new top
void stackPush(pNode **top, void *p) {
	pNode *n = stackCreateNode();
	n->p = p;
	n->prev = *top;
	*top = n;
}

bool stackEmpty(pNode **top) {
	return (*top)->prev == NULL;
}

// Pops the top element of a stack and returns the new top
void stackPop(pNode **top) {
	if ((*top)->prev == NULL) {
		// At root, no action required
		return;
	}

	pNode *prev = (*top)->prev;
	mwfree(*top, sizeof(pNode));
	*top = prev;
}

// Creates a new empty node
pNode *stackCreateNode() {
	pNode *root = (pNode *) mwalloc(sizeof(pNode));
	root->prev = NULL;
	root->p = NULL;
	return root;
}

void stackDump(pNode **top) {
	pNode *next = *top;

	const int COL = STACK_PRINT_COLS;

	printf("[STACK] Dumping stack at %p\n", *top);

	for (int i = 0; i < 20 * COL - 1; i++) {
		printf("-");
	}
	printf("\n");
	int i = 0;
	int r = 0;
	while (next != NULL) {
		printf("[%2d] %p ", r * COL + i, next->p);

		next = next->prev;

		if (next != NULL) {
			printf("-> ");
		} else {
			printf(" [END]\n");
		}

		i = (i + 1) % COL;
		if (i == 0) {
			r++;
			printf("\n");
		}
	}

	for (int i = 0; i < 20 * COL - 1; i++) {
		printf("-");
	}
	printf("\n");
}
