#pragma once

#include <stdbool.h>

#define TOKEN_N 33

enum{
	ERROR = -1,				// -1 Illegal token
	// Terminal symbols
    LITERAL_STRING,         //  0 String literal
    LITERAL_INT,            //  1 Integer literal
    LITERAL_FLOAT,          //  2 Floating point literal
    LITERAL_HEX,            //  3 Hexadecimal integer literal
    LITERAL_FLOAT_HEX,      //  4 Hexadecimal floating point literal
	SYMBOL_PLUS,			//  5 Plus symbol '+'
	SYMBOL_MINUS,			//  6 Minus symbol '-'
	SYMBOL_MULTIPLY,		//  7 Times symbol '*'
	SYMBOL_DIVIDE,			//  8 Divides symbol '/'
	SYMBOL_EQUAL,			//  9 Equals symbol '='
	SYMBOL_LPAREN,			// 10 Left parentheses '('
	SYMBOL_RPAREN,			// 11 Right parentheses ')'
	SYMBOL_LSBRACKET,		// 12 Left square brackets '['
	SYMBOL_RSBRACKET,		// 13 Right square brackets ']'
	SYMBOL_LCBRACKET,		// 14 Left curly brackets '{'
	SYMBOL_RCBRACKET,		// 15 Right curly brackets '{'
	CHAR_NUL,				// 16 NUL character, represents that the lexer has reached the end of input.
	// Non-terminal symbols
	SYM_S,					// 17 Start symbol
	SYM_ACT,				// 18 Action symbol
	SYM_BODY,				// 19 Body symbol
	SYM_EXPR,				// 20 Expression symbol
	SYM_TERM_Z,				// 21 Term-z symbol
	SYM_EXPR_OP,			// 22 Expression operator symbol
	SYM_TERM,				// 23 Term symbol
	SYM_FACTOR_Z,			// 24 Factor-z symbol
	SYM_TERM_OP,			// 25 Term operator symbol
	SYM_FACTOR,				// 26 Factor symbol
	SYM_FUNC,				// 27 Function symbol
	SYM_NUMBER,				// 28 Number symbol
	SYM_ID,					// 29 Identifier symbol
	SYM_FUNC_BODY,			// 30 Function body symbol
	SYM_FUNC_ARG,			// 31 Function argument symbol
	EPSILON,				// 32 Epsilon
} typedef tokenType;

extern const char *tokenTypeName[];

typedef struct token{
    tokenType type;
    char *value;
} token;

void tokenCopy(token *src, token *dest);
bool isTerminal(tokenType t);
bool isTerminalStrict(tokenType t);
