#pragma once

#include "parser.h"

parseTreeNode *parseTreePrune(parseTreeNode *root);
parseTreeNode *parseTreeDeallocateEpsilon(parseTreeNode *root);
