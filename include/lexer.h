#pragma once
#include "token.h"
#include "stdlib.h"
#include <stdbool.h>

// Lexer states
enum{
    DEFAULT, 		// 0
	NUMBER,			// 1
	STRING,			// 2
} typedef lxState;

static const char *lxStateName[] = {
    "DEFAULT",
	"NUMBER",
	"STRING"
};

typedef struct lxStack{
	lxState state;
	struct lxStack *prev;
} lxStack;

// State stack functions
lxStack *lexerInitStack();
void lexerDestroyStack();

void lexerPushStack(lxState state);
lxState lexerPopStack();
lxState lexerState();

// Lexer state functions
token lexerDefaultState(char *input, int *index); 			// 0
token lexerNumberLiteralState(char *input, int *index); 	// 1
token lexerStringLiteralState(char *input, int *index); 	// 2

// Initialize resources
void initLexer(char *input);
// Free resources
void deinitLexer();

token lexerPeek();
token lexerNext();
token enterState(lxState state);
void exitState();

bool lexerIgnoreWhitespace(int *index, token *t);
void lexerCatchAll(int *index, token *t);

// Globals
extern token (*lexerStates[])(char *, int *);
