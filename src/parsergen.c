#include <parsergen.h>
#include <stdio.h>
#include <switches.h>

// Remember to adjust the size of these arrays if you add rules and tokens
// RULE_N is defined in parsergen.h
// TOKEN_N is defined in token.h
parserRule parserRules[RULE_N];
bool parserFirstSet[TOKEN_N][TOKEN_N] = { { false } };
bool parserFollowSet[TOKEN_N][TOKEN_N] = { { false } };
int parserPredictTable[TOKEN_N][TOKEN_N] = { { -1 } };

static bool parserStateOk = true;

bool parserGenOk() {
	return parserStateOk;
}

void parserInitTables() {
	if (PRINT_PARSERGEN_DEBUG_MSGS) {
		printf("[PSGEN] Initializing parse tables.\n");
	}
	for (int i = 0; i < TOKEN_N; i++) {
		for (int j = 0; j < TOKEN_N; j++) {
			parserPredictTable[i][j] = -1;
		}
	}
}

void parserInitRules() {
	if (PRINT_PARSERGEN_DEBUG_MSGS) {
		printf("[PSGEN] Adding parser rules.\n");
	}
	parserAddRule(0, SYM_S, SYM_EXPR, SYM_ACT, -1); 				// S -> expr act
	parserAddRule(1, SYM_ACT, SYMBOL_EQUAL, SYM_BODY, -1); 			// act -> = body
	parserAddRule(2, SYM_ACT, EPSILON, -1, -1); 					// act -> epsilon
	parserAddRule(3, SYM_BODY, SYM_EXPR, -1, -1); 					// body -> expr
	parserAddRule(4, SYM_BODY, EPSILON, -1, -1); 					// body -> epsilon
	parserAddRule(5, SYM_EXPR, SYM_TERM, SYM_TERM_Z, -1);			// expr -> term term-z
	parserAddRule(6, SYM_TERM_Z, SYM_EXPR_OP, SYM_TERM, SYM_TERM_Z); 				// term-z -> expr-op term term-z
	parserAddRule(7, SYM_TERM_Z, EPSILON, -1, -1); 					// term-z -> epsilon
	parserAddRule(8, SYM_EXPR_OP, SYMBOL_PLUS, -1, -1); 			// expr-op -> +
	parserAddRule(9, SYM_EXPR_OP, SYMBOL_MINUS, -1, -1); 			// expr-op -> -
	parserAddRule(10, SYM_TERM, SYM_FACTOR, SYM_FACTOR_Z, -1); 		// term -> factor factor-z
	parserAddRule(11, SYM_FACTOR_Z, SYM_TERM_OP, SYM_FACTOR, SYM_FACTOR_Z);			// factor-z -> term-op factor factor-z
	parserAddRule(12, SYM_FACTOR_Z, EPSILON, -1, -1);				// factor-z -> epsilon
	parserAddRule(13, SYM_TERM_OP, SYMBOL_MULTIPLY, -1, -1); 		// term-op -> *
	parserAddRule(14, SYM_TERM_OP, SYMBOL_DIVIDE, -1, -1); 			// term-op -> /
	parserAddRule(15, SYM_FACTOR, SYM_FUNC, -1, -1);				// factor -> func
	parserAddRule(16, SYM_FACTOR, SYMBOL_LPAREN, SYM_EXPR, SYMBOL_RPAREN); 			// factor -> ( expr )
	parserAddRule(17, SYM_FACTOR, SYMBOL_LSBRACKET, SYM_EXPR, SYMBOL_RSBRACKET); 	// factor -> [ expr ]
	parserAddRule(18, SYM_FACTOR, SYMBOL_LCBRACKET, SYM_EXPR, SYMBOL_RCBRACKET); 	// factor -> { expr }
	parserAddRule(19, SYM_FACTOR, SYM_NUMBER, -1, -1); 				// factor -> number
	parserAddRule(20, SYM_FACTOR, SYMBOL_MINUS, SYM_NUMBER, -1); 	// factor -> - number
	parserAddRule(21, SYM_FUNC, SYM_ID, SYM_FUNC_BODY, -1); 		// func -> id func-body
	parserAddRule(22, SYM_NUMBER, LITERAL_INT, -1, -1); 			// number -> int
	parserAddRule(23, SYM_NUMBER, LITERAL_HEX, -1, -1); 			// number -> hex
	parserAddRule(24, SYM_NUMBER, LITERAL_FLOAT, -1, -1); 			// number -> float
	parserAddRule(25, SYM_NUMBER, LITERAL_FLOAT_HEX, -1, -1); 		// number -> float_hex
	parserAddRule(26, SYM_ID, LITERAL_STRING, -1, -1); 				// id -> string
	parserAddRule(27, SYM_FUNC_BODY, SYMBOL_LPAREN, SYM_FUNC_ARG,
			SYMBOL_RPAREN); 										// func-body -> ( func-arg )
	parserAddRule(28, SYM_FUNC_BODY, EPSILON, -1, -1); 				// func-body -> epsilon
	parserAddRule(29, SYM_FUNC_ARG, SYM_EXPR, -1, -1); 				// func-arg -> expr
	parserAddRule(30, SYM_FUNC_ARG, EPSILON, -1, -1); 				// func-arg -> epsilon
}

void parserAddRule(int index, tokenType goal, tokenType e0, tokenType e1,
		tokenType e2) {
	if (goal == e0) {
		// Left recursion
		printf("[PSGEN] Possible left recursion detected! Ignoring rule.\n");
		parserStateOk = false;
		return;
	}

	parserRules[index].goal = goal;
	parserRules[index].expr[0] = e0;
	parserRules[index].expr[1] = e1;
	parserRules[index].expr[2] = e2;

	if (PRINT_PARSERGEN_DEBUG_MSGS) {
		parserDisplayRule(index);
	}
}

void parserGenerateFirstSet() {
	bool setChanged;
	int pass = 1;

	// Init set
	for (int i = 0; i < TOKEN_N; i++) {
		if (isTerminal(i)) {
			parserFirstSet[i][i] = true;
		}

		if (i == EPSILON) {
			parserFirstSet[i][i] = true;
		}
	}

	do {
		setChanged = false;

		// For each token
		for (int i = 0; i < TOKEN_N; i++) {
			// That is not a terminal
			if (!isTerminal(i)) {
				bool allHasEpsilon = true;
				for (int j = 0; j < RULE_N; j++) {
					// Rules with i on the left side
					if (parserRules[j].goal == i) {
						if (parserRules[j].expr[0] == EPSILON) {
							parserFirstSet[i][EPSILON] = true;
							continue;
						}

						// First expression has epsilon
						bool epsilonInPrev = parserSetHasEpsilon(
								parserFirstSet[parserRules[j].expr[0]]);
						allHasEpsilon &= epsilonInPrev;
						// Union first set of first expression excluding epsilon
						setChanged |= parserUnionSetComplEpsilon(
								parserFirstSet[parserRules[j].expr[0]],
								parserFirstSet[i]);

						for (int k = 1; k < EXPR_N; k++) {
							if (parserRules[j].expr[k] == -1) {
								break;
							}

							if (allHasEpsilon) {
								// If all the previous expression has epsilon, union first set of current expression excluding epsilon
								setChanged |= parserUnionSetComplEpsilon(
										parserFirstSet[parserRules[j].expr[k]],
										parserFirstSet[i]);
							}

							// Update variables
							epsilonInPrev = parserSetHasEpsilon(
									parserFirstSet[parserRules[j].expr[k]]);
							allHasEpsilon &= epsilonInPrev;
						}
					}
				}

				// If all expression in rules which i is on the left size have epsilon in it, add epsilon
				if (allHasEpsilon && !parserFirstSet[i][EPSILON]) {
					parserFirstSet[i][EPSILON] = true;
					setChanged = true;
				}
			}
		}

		//printf("[DEBUG] setChanged = %d\n", setChanged);
		//parserDisplayFirstSet();
		//printf("[DEBUG] First set generated at this point (pass #%2d) displayed. [press enter]", pass++);
		//getchar();
		if (PRINT_PARSERGEN_DEBUG_MSGS) {
			printf("[PSGEN] First set generation pass #%02d\n", pass++);
		}
		// Safe guard :P
		if (pass > 500) {
			printf("[PSGEN] First set generation exceeded maximum passes!\n");
			parserStateOk = false;
		}
	} while (setChanged); // Stop if nothing has changed
}

void parserGenerateFollowSet() {
	bool setChanged;
	int pass = 1;

	// Init set
	parserFollowSet[SYM_S][CHAR_NUL] = true;

	do {
		setChanged = false;

		for (int i = 0; i < TOKEN_N; i++) {
			if (!isTerminal(i)) {
				// Foreach non-terminal
				for (int j = 0; j < RULE_N; j++) {
					// Rules with i on the right side
					if (parserRuleHasTokenInExpr(j, i)) {
						for (int k = 0; k < EXPR_N; k++) {
							// Rules that use up to k + 1
							if (k + 1 < EXPR_N) {
								// Foreach X -> i beta
								if (parserRules[j].expr[k] == i
										&& parserRules[j].expr[k + 1] != -1) {
									bool betaHasEpsilon = parserSetHasEpsilon(
											parserFirstSet[parserRules[j].expr[k
													+ 1]]);
									setChanged |= parserUnionSetComplEpsilon(
											parserFirstSet[parserRules[j].expr[k
													+ 1]], parserFollowSet[i]);

									// If epsilon is in Fi(beta)
									if (betaHasEpsilon) {
										setChanged |=
												parserUnionSetComplEpsilon(
														parserFollowSet[parserRules[j].goal],
														parserFollowSet[i]);
									}
								}

								// Foreach X -> i
								if (parserRules[j].expr[k] == i
										&& parserRules[j].expr[k + 1] == -1) {
									setChanged |=
											parserUnionSetComplEpsilon(
													parserFollowSet[parserRules[j].goal],
													parserFollowSet[i]);
								}
							}
						}
					}
				}
			}
		}

		//printf("[DEBUG] setChanged = %d\n", setChanged);
		//parserDisplayFollowSet();
		//printf("[DEBUG] Follow set generated at this point (pass #%2d) displayed. [press enter]", pass++);
		//getchar();
		if (PRINT_PARSERGEN_DEBUG_MSGS) {
			printf("[PSGEN] Follow set generation pass #%02d\n", pass++);
		}
		// Safe guard :P
		if (pass > 500) {
			printf("[PSGEN] Follow set generation exceeded maximum passes!\n");
			parserStateOk = false;
		}
	} while (setChanged); // Run until all the sets stay the same
}

void parserGeneratePredictTable() {
	if (PRINT_PARSERGEN_DEBUG_MSGS) {
		printf("[PSGEN] Generating parse table.\n");
	}

	// For each rule A -> t
	for (int i = 0; i < RULE_N; i++) {
		// For each terminal a
		for (int j = 0; j <= CHAR_NUL; j++) {
			int k = 0;
			// a is int Fi(t)
			if (parserFirstSet[parserRules[i].expr[k]][j]) {
				if (parserPredictTable[parserRules[i].goal][j]
						!= i&& parserPredictTable[parserRules[i].goal][j] != -1 && parserPredictTable[parserRules[i].goal][j] < RULE_N) {
					printf("[PSGEN] Parse table conflict!\n");
					printf(
							"[PSGEN] Conflict in parseTable[%17s][%17s] at rules:\n",
							tokenTypeName[parserRules[i].goal + 1],
							tokenTypeName[j + 1]);
					parserDisplayRule(
							parserPredictTable[parserRules[i].goal][j]);
					parserDisplayRule(i);
					parserStateOk = false;
					goto endGeneratePredictTable;
				}
				// Add rule to table
				parserPredictTable[parserRules[i].goal][j] = i;
			}

			// epsilon is in Fi(t) and a is in Fo(t)
			if (parserSetHasEpsilon(
					parserFirstSet[parserRules[i].expr[k]]) && parserFollowSet[parserRules[i].goal][j] && parserPredictTable[parserRules[i].goal][j] < RULE_N) {
				if (parserPredictTable[parserRules[i].goal][j] != i
						&& parserPredictTable[parserRules[i].goal][j] != -1) {
					printf("[PSGEN] Parse table conflict!\n");
					printf(
							"[PSGEN] Conflict in parseTable[%17s][%17s] at rules:\n",
							tokenTypeName[parserRules[i].goal + 1],
							tokenTypeName[j + 1]);
					parserDisplayRule(
							parserPredictTable[parserRules[i].goal][j]);
					parserDisplayRule(i);
					parserStateOk = false;
					goto endGeneratePredictTable;
				}
				// Add rule to table
				parserPredictTable[parserRules[i].goal][j] = i;
			}
		}
	}

	endGeneratePredictTable: return;
}

bool parserRuleHasTokenInExpr(int index, tokenType t) {
	bool ret = false;
	for (int i = 0; i < EXPR_N; i++) {
		ret |= parserRules[index].expr[i] == t;
	}

	return ret;
}

bool parserUnionSetComplEpsilon(bool *from, bool *to) {
	bool changed = false;
	for (int i = 0; i < TOKEN_N; i++) {
		if (i == EPSILON)
			continue;
		if (from[i] != to[i] && from[i]) {
			changed = true;
			to[i] = from[i];
		}
	}

	return changed;
}

bool parserSetHasEpsilon(bool *set) {
	return set[EPSILON];
}

void parserDisplayRule(int index) {
	printf("[PSGEN] Rule #%02d: %17s -> %s", index,
			tokenTypeName[parserRules[index].goal + 1],
			tokenTypeName[parserRules[index].expr[0] + 1]);
	if (parserRules[index].expr[1] != -1) {
		printf(" %s", tokenTypeName[parserRules[index].expr[1] + 1]);

		if (parserRules[index].expr[2] != -1) {
			printf(" %s", tokenTypeName[parserRules[index].expr[2] + 1]);
		}
	}
	printf("\n");
}

void parserDisplayFirstSet() {
	for (int i = 0; i < TOKEN_N; i++) {
		printf("[PSGEN] Fi(%17s) = {", tokenTypeName[i + 1]);
		bool pc = false;
		for (int j = 0; j < TOKEN_N; j++) {
			if (parserFirstSet[i][j]) {
				if (pc) {
					printf(", ");
				} else {
					pc = true;
				}

				printf("%s", tokenTypeName[j + 1]);
			}
		}
		printf("}\n");
	}
}

void parserDisplayFollowSet() {
	for (int i = 0; i < TOKEN_N; i++) {
		if (isTerminal(i))
			continue;
		if (i == EPSILON)
			continue;
		printf("[PSGEN] Fo(%17s) = {", tokenTypeName[i + 1]);
		bool pc = false;
		for (int j = 0; j < TOKEN_N; j++) {
			if (parserFollowSet[i][j]) {
				if (pc) {
					printf(", ");
				} else {
					pc = true;
				}

				printf("%s", tokenTypeName[j + 1]);
			}
		}
		printf("}\n");
	}
}

void parserDisplayPredictTable() {
	const int COL = PARSER_GEN_PRINT_COLS;
	for (int i = 0; i <= CHAR_NUL / COL; i++) {
		printf("[PSGEN] Parse table page %d out of %d\n", i + 1,
				CHAR_NUL / COL + 1);
		for (int j = 0; j < 13 + COL * 18; j++) {
			printf("-");
		}
		printf("\n");

		printf("              "); // 14
		for (int j = 0; j + i * COL <= CHAR_NUL && j < COL; j++) {
			// Header
			printf("%17s ", tokenTypeName[j + i * COL + 1]);
		}
		printf("\n");

		for (int j = CHAR_NUL + 1; j < TOKEN_N - 1; j++) {
			// Row header
			printf("%13s ", tokenTypeName[j + 1]);

			// Columns
			for (int k = 0; k + i * COL <= CHAR_NUL && k < COL; k++) {
				if (parserPredictTable[j][k + i * COL] != -1) {
					printf("         Rule #%02d ",
							parserPredictTable[j][k + i * COL]);
				} else {
					printf("                - ");
				}
			}
			printf("\n");
		}

		for (int j = 0; j < 13 + COL * 18; j++) {
			printf("-");
		}
		printf("\n");
	}
}
