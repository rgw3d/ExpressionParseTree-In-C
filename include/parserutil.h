#pragma once

#include "parser.h"

/**
 * Does nothing
 *
 * @param root
 * @return
 */
parseTreeNode *parseTreePrune(parseTreeNode *root);
/**
 * Prunes branches that terminate in EPSILON.
 *
 * @param root the pointer to the root of the parse tree.
 * @return new location of the new root.
 */
parseTreeNode *parseTreeDeallocateEpsilon(parseTreeNode *root);
