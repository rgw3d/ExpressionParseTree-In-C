#pragma once

enum{
	ERROR = -1,				// -1 Illegal token
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
} typedef tokenType;

static const char *tokenTypeName[] = {
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
	"CHAR_NUL"
};

typedef struct token{
    tokenType type;
    char *value;
} token;

void tokenCopy(token *src, token *dest);
