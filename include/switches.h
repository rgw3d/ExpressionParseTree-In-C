#pragma once

/**
 * Whether to pause the program at specified locations.
 */
#define MAIN_BREAK 1

/**
 * Whether to print memory statistics.
 */
#define PRINT_MEM_STATS 1

/**
 * Whether to print a message whenever memory has been allocated/deallocated.
 */
#define PRINT_MEM_DEBUG_MSGS 0

/**
 * Whether to prompt for input when the parser processes a single token.
 */
#define PAUSE_PARSER_ON_STEP 0

/**
 * Whether to print the generated sets and tables by the parser generator.
 */
#define PRINT_PARSERGEN_DEBUG_MSGS 1

/**
 * The number of elements in a stack to be printed on one line by the stackDump
 * function.
 */
#define STACK_PRINT_COLS 10

/**
 * The number of columns to print on a page by the parserDisplayPredictTable.
 */
#define PARSER_GEN_PRINT_COLS 10

/**
 * Auto generate svg files from dot files with system call.
 */
#define PARSER_GEN_SVG_FROM_DOT 0
