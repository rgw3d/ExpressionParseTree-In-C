#pragma once

#include <stdbool.h>

typedef struct pNode{
	void *p;
	struct pNode *prev;
} pNode;

bool stackEmpty(pNode **top);
void stackDestroy(pNode **top);
void stackPush(pNode **top, void *p);
void stackPop(pNode **top);
pNode *stackCreateNode();

void stackDump(pNode **top);
