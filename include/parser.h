#pragma once

#include "token.h"
#include "parsergen.h"
#include <stdbool.h>

#define PAUSE_PARSER_ON_STEP 1

typedef struct parseTreeNode{
	struct parseTreeNode *parent;
	tokenType type;
	char *value;
	parserRule *rule;
	int childCount;
	struct parseTreeNode *children[];
} parseTreeNode;

void parserInit();

void parseTreeCreateRoot();
void parseTreeCreateNode(parseTreeNode *parent, int index, tokenType type);
parseTreeNode *parseTreeNodeRealloc(parseTreeNode *node, int newChildCount);
parseTreeNode *parseTreeApplyRule(parseTreeNode *node, parserRule *rule);
void parseTreeCopyValue(parseTreeNode *node, char* str);
void parseTreeDestroy();

bool parserOk();

parseTreeNode *parserExec();

void parseTreeNodeCopy(parseTreeNode *from, parseTreeNode *to);
