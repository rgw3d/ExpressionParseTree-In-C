#include "token.h"
#include <stdlib.h>

void tokenCopy(token *src, token *dest){
	dest->type = src->type;
	dest->value = src->value;
}

const char *tokenTypeName[] = {
    "ERROR",
	"LITERAL_STRING",
	"LITERAL_INT",
	"LITERAL_FLOAT",
	"LITERAL_HEX",
	"LITERAL_FLOAT_HEX",
	"SYMBOL_PLUS",
	"SYMBOL_MINUS",
	"SYMBOL_MULTIPLY",
	"SYMBOL_DIVIDE",
	"SYMBOL_EQUAL",
	"SYMBOL_LPAREN",
	"SYMBOL_RPAREN",
	"SYMBOL_LSBRACKET",
	"SYMBOL_RSBRACKET",
	"SYMBOL_LCBRACKET",
	"SYMBOL_RCBRACKET",
	"CHAR_NUL",
	"SYM_S",
	"SYM_ACT",
	"SYM_BODY",
	"SYM_EXPR",
	"SYM_TERM_Z",
	"SYM_EXPR_OP",
	"SYM_TERM",
	"SYM_FACTOR_Z",
	"SYM_TERM_OP",
	"SYM_FACTOR",
	"SYM_FUNC",
	"SYM_NUMBER",
	"SYM_ID",
	"SYM_FUNC_BODY",
	"SYM_FUNC_ARG",
	"EPSILON",
};

bool isTerminal(tokenType t){
	return t <= CHAR_NUL;
}
