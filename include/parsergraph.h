#pragma once

#include "parser.h"
#include <stdbool.h>

/**
 * Generate a parse tree graph.
 *
 * @param root a pointer to the root of the parse tree.
 * @param filename file to output the file into.
 * @param compressTerms whether to combine terminals with non-terminals.
 */
void parserGenerateDotFile(parseTreeNode *root, const char *filename, bool compressTerms);
