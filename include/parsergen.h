#pragma once

#include "token.h"
#include <stdbool.h>

#define EXPR_N 3
#define RULE_N 31

typedef struct parserRule{
	tokenType goal;
	tokenType expr[EXPR_N];
} parserRule;

extern parserRule parserRules[];
extern bool parserFirstSet[TOKEN_N][TOKEN_N];
extern bool parserFollowSet[TOKEN_N][TOKEN_N];
extern int parserPredictTable[TOKEN_N][TOKEN_N];

void parserInitTables();
void parserInitRules();
void parserAddRule(int index, tokenType goal, tokenType e0, tokenType e1, tokenType e2);
void parserGenerateFirstSet();
void parserGenerateFollowSet();
void parserGeneratePredictTable();

bool parserUnionSetComplEpsilon(bool *from, bool *to);
bool parserSetHasEpsilon(bool *set);
bool parserRuleHasTokenInExpr(int index, tokenType t);

void parserDisplayRule(int index);
void parserDisplayFirstSet();
void parserDisplayFollowSet();
void parserDisplayPredictTable();

bool parserGenOk();
