#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct sPtr {
	char* p;
	struct sPtr *prev;
} sPtr;

typedef struct ssChar {
	char c;
	struct ssChar *prev;
} ssChar;

int ssLen(ssChar *top);
char *ssStr(ssChar *top);

ssChar *ssPush(char c, ssChar *top);
ssChar *ssPop(ssChar *top);
ssChar *ssRoot(ssChar *top);

void ssDestroy(ssChar *top);
void ssCleanup();

void spInit();
void spPush(char *c);
void spPop();

void spCleanup();
void spDestroy();
