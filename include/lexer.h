#pragma once

#include <stdbool.h>

#include "token.h"

/**
 * Lexer states, represents the state that the lexer is in.
 */
typedef enum {
	DEFAULT, 		///< default lexer state.
	NUMBER,			///< lexer state when processing numbers.
	STRING,			///< lexer state when processing strings.
} lxState;

/**
 * An element of a lexer state stack. The lexer state stack is pretty much
 * useless since the depth of the lexer state never exceeds 1.
 */
typedef struct lxStack {
	lxState state;			///< hold the lexer state of that element.
	struct lxStack *prev;	///< points to the previous element in the stack.
							///< NULL if the element is the root.
} lxStack;

/**
 * Returns the string representation of the lexer state.
 *
 * @param state the state of the lexer.
 * @return a pointer to the name of the lexer state.
 */
const char *lexerStateName(lxState state);

// State stack functions
/**
 * Initializes the lexer state stack.
 *
 * @return a pointer to the root of the state stack.
 */
lxStack *lexerInitStack();
/**
 * Deallocates all memory allocated by the lexer state stack.
 */
void lexerDestroyStack();

/**
 * Adds the state to the top of the lexer state stack.
 *
 * @param state the lexer state to add to the stack.
 */
void lexerPushStack(lxState state);
/**
 * Pops the top element of the lexer state stack.
 *
 * @return the state of the top stack element.
 */
lxState lexerPopStack();
/**
 * Gets the current state of the lexer.
 *
 * @return the state of the lexer.
 */
lxState lexerState();

// Lexer state functions
/**
 * Runs the lexer in the default state.
 *
 * @param input a pointer to the start of the input.
 * @param index position of the input to start processing.
 * @return the next token in the input.
 */
token lexerDefaultState(char *input, int *index); 			// 0
/**
 * Runs the lexer and try to process numbers.
 *
 * @param input a pointer to the start of the input.
 * @param index position of the input to start processing.
 * @return the next token in the input.
 */
token lexerNumberLiteralState(char *input, int *index); 	// 1
/**
 * Runs the lexer and try to process strings.
 *
 * @param input a pointer to the start of the input.
 * @param index position of the input to start processing.
 * @return the next token in the input.
 */
token lexerStringLiteralState(char *input, int *index); 	// 2

// Initialize resources
/**
 * Initializes the lexer.
 *
 * @param input a pointer to the input.
 */
void initLexer(char *input);
// Free resources
/**
 * Deallocates memory used by the lexer.
 */
void deinitLexer();

/**
 * Get the next token without consuming it.
 *
 * @return the next token in the input.
 */
token lexerPeek();
/**
 * Get the next token and consume it.
 *
 * @return the next token in the input.
 */
token lexerNext();
/**
 * Continues the lexer in the state.
 *
 * @param state the state to enter.
 * @return the next token in the input.
 */
token enterState(lxState state);
/**
 * Make the lexer continue in the previous state in the stack on the next call.
 */
void exitState();

/**
 * Processes and ignores whitespace for the lexer.
 *
 * @param index position of the input to start processing.
 * @param t pointer to save the token to.
 * @return true if a whitespace was ignored.
 */
bool lexerIgnoreWhitespace(int *index, token *t);
/**
 * Sets the next token to adn error token if no rules in the lexer has matched
 * and if the next character is not the NUL character. If the next character is
 * the NUL character, the token type is set to CHAR_NUL.
 *
 * @param index position of the input to start processing.
 * @param t pointer to save the token to.
 */
void lexerCatchAll(int *index, token *t);

// Globals
/**
 * An array of function pointers that points to the function that is responsible
 * of processing the input in a specific lexer state.
 *
 * @param a pointer to the start of the input.
 * @param position of the input to start processing.
 * @return the next token in the input.
 */
extern token (*lexerStates[])(char *, int *);
