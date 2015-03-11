#pragma once

#include <stdbool.h>

#include "token.h"

/**
 * The maximum expression length of parser rules.
 */
#define EXPR_N 3
/**
 * The number of rules.
 */
#define RULE_N 31

/**
 * Represents a parser rule. Parser rules are defined as context-free grammars,
 * the parser generate will attempt to generate a parse table from the rules.
 *
 * The generated parser is a LL(1) parser, thus defined rules should avoid
 * having first/first and first/follow conflicts.
 */
typedef struct parserRule {
	tokenType goal;			///< Non-terminal
	tokenType expr[EXPR_N]; ///< Sequence of terminals or non-terminals
} parserRule;

/**
 * The array of parser rules. The order in which the rules are listed does not
 * affect the generate parser.
 */
extern parserRule parserRules[RULE_N];
/**
 * The first set of terminals and non-terminals generated from the parser rules.
 *
 * The first set for t is defined as the set of terminals that can be found at
 * the start of some string t, plus epsilon if the empty string belongs to t.
 */
extern bool parserFirstSet[TOKEN_N][TOKEN_N];
/**
 * The follow set of non-terminals generate from the parser rules.
 *
 * The follow set for t is defined as the set of terminals that can follow some
 * string t.
 */
extern bool parserFollowSet[TOKEN_N][TOKEN_N];
/**
 * The parse table is used to predict which rule to apply to a non-terminal
 * based on the next token in the input.
 *
 * An entry for a nonterminal A and terminal a contains a rule A -> t if and
 * only if a is in the first set of t or epsilon is in the first set of t and a
 * is in the follow set of A.
 */
extern int parserPredictTable[TOKEN_N][TOKEN_N];

/**
 * Initializes the tables used by the parser generator with default values.
 */
void parserInitTables();
/**
 * Loads the parser rules into the rules array.
 */
void parserInitRules();
/**
 * Adds a rule in the rules array at the provided index.
 *
 * @param index position to insert the rule.
 * @param goal the non-terminal expected from an expression.
 * @param e0 the first terminal in the expression.
 * @param e1 the second terminal in the the expression, set to -1 for none.
 * @param e2 the third terminal in the expression, set to -1 for none.
 */
void parserAddRule(int index, tokenType goal, tokenType e0, tokenType e1,
		tokenType e2);
/**
 * Generates the first set from the parser rules.
 */
void parserGenerateFirstSet();
/**
 * Generates the follow set from the parser rules.
 */
void parserGenerateFollowSet();
/**
 * Generates the parse table from the first set and follow set.
 */
void parserGeneratePredictTable();

/**
 * Unions two sets excluding epsilons.
 *
 * @param from a pointer to the set to read from.
 * @param to a pointer to the set to write the union to.
 * @return true if the destination set has been changed. Use this value to
 * 		   determine whether to continue generating the first or follow set.
 */
bool parserUnionSetComplEpsilon(bool *from, bool *to);
/**
 * Whether a set has epsilon in it.
 *
 * @param set a pointer to the set to check.
 * @return true if epsilon is an element of the set.
 */
bool parserSetHasEpsilon(bool *set);
/**
 * Whether a rule has a token in its expression.
 *
 * @param index the index of the rule in the rule array.
 * @param t the type of the token to check.
 * @return true if t is an element in a rule's expression.
 */
bool parserRuleHasTokenInExpr(int index, tokenType t);

/**
 * Prints out a rule.
 *
 * @param index the index of the rule to print in the rule array.
 */
void parserDisplayRule(int index);
/**
 * Prints out the first set.
 */
void parserDisplayFirstSet();
/**
 * Prints out the follow set.
 */
void parserDisplayFollowSet();
/**
 * Prints out the parse table.
 */
void parserDisplayPredictTable();

/**
 * Checks whether the parser generator is in an expected state.
 *
 * @return false if the parser generator has encountered an error.
 */
bool parserGenOk();
