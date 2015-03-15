#pragma once

#include <stdbool.h>

/**
 * The number of defined tokens excluding, should be set to the integer value
 * of EPSILON + 1.
 */
#define TOKEN_N 33

/**
 * Defines tokens used by the lexer and parser.
 */
typedef enum {
	ERROR = -1,				///< Illegal token.
	// Terminal symbols
	LITERAL_STRING,         ///< String literal.
	LITERAL_INT,            ///< Integer literal.
	LITERAL_FLOAT,          ///< Floating point literal.
	LITERAL_HEX,            ///< Hexadecimal integer literal.
	LITERAL_FLOAT_HEX,      ///< Hexadecimal floating point literal.
	SYMBOL_PLUS,			///< Plus symbol '+'.
	SYMBOL_MINUS,			///< Minus symbol '-'.
	SYMBOL_MULTIPLY,		///< Times symbol '*'.
	SYMBOL_DIVIDE,			///< Divides symbol '/'.
	SYMBOL_EQUAL,			///< Equals symbol '='.
	SYMBOL_LPAREN,			///< Left parentheses '('.
	SYMBOL_RPAREN,			///< Right parentheses ')'.
	SYMBOL_LSBRACKET,		///< Left square brackets '['.
	SYMBOL_RSBRACKET,		///< Right square brackets ']'.
	SYMBOL_LCBRACKET,		///< Left curly brackets '{'.
	SYMBOL_RCBRACKET,		///< Right curly brackets '{'.
	CHAR_NUL,				///< NUL character, represents that the lexer has
							///< reached the end of input. Tokens defined before
							///< CHAR_NUL is considered as terminal tokens,
							///< tokens defined after this is considered as
							///< non-terminal tokens. To check whether a token
							///< is terminal or non-terminal use the isTerminal
							///< or isTerminalStrict functions.
	// Non-terminal symbols
	SYM_S,					///< Start symbol.
	SYM_ACT,				///< Action symbol.
	SYM_BODY,				///< Body symbol.
	SYM_EXPR,				///< Expression symbol.
	SYM_TERM_Z,				///< Term-z symbol.
	SYM_EXPR_OP,			///< Expression operator symbol.
	SYM_TERM,				///< Term symbol.
	SYM_FACTOR_Z,			///< Factor-z symbol.
	SYM_TERM_OP,			///< Term operator symbol.
	SYM_FACTOR,				///< Factor symbol.
	SYM_FUNC,				///< Function symbol.
	SYM_NUMBER,				///< Number symbol.
	SYM_ID,					///< Identifier symbol.
	SYM_FUNC_BODY,			///< Function body symbol.
	SYM_FUNC_ARG,			///< Function argument symbol.
	EPSILON,				///< Epsilon, represents an empty string. This token
							///< should always be defined last.
} tokenType;

/**
 * An array of the names of tokens. Since the ERROR token is defined with the
 * integer value of -1, to get the correct name for a token, query this array
 * with the integer value of a token plus 1.
 *
 * TODO: move functionality to a function to avoid the need +1 to the token val.
 */
extern const char *tokenTypeName[];

/**
 * Represents a token.
 */
typedef struct token {
	tokenType type;	///< the type of a token.
	char *value;	///< the value of a token, NULL if the token does not have
					///< a value.
} token;

/**
 * Copies the members of the token struct from one to another.
 *
 * @param src the source token.
 * @param dest the destination token.
 */
void tokenCopy(token *src, token *dest);
/**
 * Checks whether a token is terminal, CHAR_NUL is considered as a terminal
 * token when using this function, to exclude CHAR_NUL from the list of terminal
 * tokens, use isTerminalStrict().
 *
 * @param t the token to test.
 * @return true if the token is a terminal token.
 */
bool isTerminal(tokenType t);
/**
 * Checks whether a token is terminal, CHAR_NUL is not considered as a terminal
 * when using this function.
 *
 * @see isTerminal
 * @param t the token to test.
 * @return true if the token is a terminal token.
 */
bool isTerminalStrict(tokenType t);
