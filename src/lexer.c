#include <ctype.h>
#include <lexer.h>
#include <memwrapper.h>
#include <stringstack.h>

static int lexerIndex;
static lxStack *lexerStackTop = NULL;
static char *inputString;

static const char *lxStateName[] = { "DEFAULT", "NUMBER", "STRING" };

// Globals
token (*lexerStates[])(char *,
		int *) = {&lexerDefaultState, &lexerNumberLiteralState, &lexerStringLiteralState
};

const char *lexerStateName(lxState state) {
	return lxStateName[state];
}

void initLexer(char *input) {
	// Initializes the variables used by the lexical analyzer
	inputString = input;
	// Initializes the lexer state stack
	lexerStackTop = (lxStack *) mwalloc(sizeof(lxStack));
	lexerStackTop->state = DEFAULT;
	lexerStackTop->prev = NULL;
}

void deinitLexer() {
	// Deallocates the lexer state stack
	lexerDestroyStack();
}

token lexerPeek() {
	// Set the original index
	int initIndex = lexerIndex;
	// Read the next token
	token ret = lexerNext();
	// Restore the original index
	lexerIndex = initIndex;
	return ret;
}

token lexerNext() {
	// Execute the lexer in the current lexer state and return the result.
	return lexerStates[lexerState()](inputString, &lexerIndex);
}

token lexerDefaultState(char *input, int *index) {
	token t;
	t.type = ERROR;
	t.value = NULL;

	lxDefaultStart:
	// Goto start if next character is a whitespace
	if (lexerIgnoreWhitespace(index, &t))
		goto lxDefaultStart;

	// Check if is character is string or if is number
	if (isdigit((int )input[*index])) {
		// Continue the lexer in a different state
		return enterState(NUMBER);
	} else if (isalpha((int )input[*index])) {
		// Continue the lexer in a different state
		return enterState(STRING);
	}

	switch (input[*index]) {
	case '+':
		t.type = SYMBOL_PLUS;
		goto lxDefaultSymbolEnd;
	case '-':
		t.type = SYMBOL_MINUS;
		goto lxDefaultSymbolEnd;
	case '*':
		t.type = SYMBOL_MULTIPLY;
		goto lxDefaultSymbolEnd;
	case '/':
		t.type = SYMBOL_DIVIDE;
		goto lxDefaultSymbolEnd;
	case '=':
		t.type = SYMBOL_EQUAL;
		goto lxDefaultSymbolEnd;
	case '(':
		t.type = SYMBOL_LPAREN;
		goto lxDefaultSymbolEnd;
	case ')':
		t.type = SYMBOL_RPAREN;
		goto lxDefaultSymbolEnd;
	case '[':
		t.type = SYMBOL_LSBRACKET;
		goto lxDefaultSymbolEnd;
	case ']':
		t.type = SYMBOL_RSBRACKET;
		goto lxDefaultSymbolEnd;
	case '{':
		t.type = SYMBOL_LCBRACKET;
		goto lxDefaultSymbolEnd;
	case '}':
		t.type = SYMBOL_RCBRACKET;
		goto lxDefaultSymbolEnd;
	}

	lexerCatchAll(index, &t);

	goto lxDefaultEnd;

	// Found symbol
	// Using labels and gotos to simplify stuff a bit
	lxDefaultSymbolEnd: (*index)++; // Advance index

	lxDefaultEnd: return t;
}

token lexerNumberLiteralState(char *input, int *index) {
	int startIndex = *index;
	token t;
	t.type = LITERAL_INT;
	t.value = NULL;

	// Create new string
	ssChar *top = NULL;

	lxNumberStart:
	// Check if the next character is a digit
	// Casting the argument to ctype functions to int since the compiler will
	// throw -Wchar-subscripts warning when compiling with -Wall.
	if (isdigit((int )input[*index])
			|| ((t.type == LITERAL_HEX || t.type == LITERAL_FLOAT_HEX)
					&& isxdigit((int )input[*index]))) {
		top = ssPush(input[*index], top); // Push character to stack
		(*index)++;
		goto lxNumberStart;
	}

	// Check if the processed number should be a in base 16
	if (input[*index] == 'x' || input[*index] == 'X') {
		if (input[startIndex] == '0' && *index == startIndex + 1) {
			top = ssPush(input[*index], top); // Push character to stack
			(*index)++; // Advance index
			// Set token type to hex literal
			t.type = LITERAL_HEX;
			goto lxNumberStart;
		} else {
			// Unexpected character, stop token here
			goto lxNumberEnd;
		}
	}

	if (input[*index] == '.') {
		// Unexpected character, stop token here
		if (t.type == LITERAL_FLOAT_HEX || t.type == LITERAL_FLOAT) {
			goto lxNumberEnd;
		}

		top = ssPush(input[*index], top); // Push character to stack
		(*index)++; // Advance index

		// Set token type to float
		if (t.type == LITERAL_HEX) {
			t.type = LITERAL_FLOAT_HEX;
		} else {
			t.type = LITERAL_FLOAT;
		}
		goto lxNumberStart;
	}

	// Discard digit separators
	if (input[*index] == ',') {
		// Discard
		(*index)++;
		goto lxNumberStart;
	}

	// Exit state if unmatched
	lxNumberEnd:
	// Create string, this string will be deallocated by ssCleanup called in cleanup in main.c
	if (top != NULL) {
		t.value = ssStr(top);
	}

	// Deallocate stack
	ssDestroy(top);

	// Exit state
	exitState();

	return t;
}

token lexerStringLiteralState(char *input, int *index) {
	token t;
	t.type = LITERAL_STRING;
	t.value = NULL;

	// Create new string
	ssChar *top = NULL;

	lxStringStart: if (isalpha((int)input[*index]) || input[*index] == '_') {
		top = ssPush(input[*index], top); // Push character to stack
		(*index)++; // Advance index
		goto lxStringStart;
	}

	// Exit state if unmatched
//	lxStringEnd:
	// Create string, this string will be deallocated by ssCleanup called in cleanup in main.c
	if (top != NULL) {
		t.value = ssStr(top);
	}

	// Deallocate stack
	ssDestroy(top);

	// Exit state
	exitState();
	return t;
}

token enterState(lxState state) {
	lexerPushStack(state);
	return lexerNext();
}

void exitState() {
	lexerPopStack();
}

lxStack *lexerInitStack() {
	// Allocate memory for the root element
	lexerStackTop = (lxStack *) mwalloc(sizeof(lxStack));

	// Initialize the lexer in the default state
	lexerStackTop->state = DEFAULT;

	// The root element should not have any element before it
	lexerStackTop->prev = NULL;

	// Return the pointer the the root element
	return lexerStackTop;
}

void lexerDestroyStack() {
	lxStack *next = lexerStackTop;
	// While there are still elements to visit
	while (next != NULL) {
		// Get the element to destroy
		lxStack *destroy = next;

		// Get the next element to visit
		next = next->prev;

		// Free allocated resources
		mwfree(destroy, sizeof(lxStack));
	}

	// Set the top element to NULL
	lexerStackTop = NULL;
}

void lexerPushStack(lxState state) {
	// Allocate memory for a new element
	lxStack *next = (lxStack *) mwalloc(sizeof(lxStack));

	// Set the previous element to the current top element
	next->prev = lexerStackTop;
	next->state = state;

	// Replace the top element
	lexerStackTop = next;
}

lxState lexerPopStack() {
	// Replace top element with the one before it
	lxStack *top = lexerStackTop;
	lexerStackTop = top->prev;

	// Get the state stored in the original top element
	lxState ret = top->state;

	// Free dynamically allocated memory
	mwfree(top, sizeof(lxStack));

	// Return the state
	return ret;
}

lxState lexerState() {
	// Return the state stored in the top element of the stack
	return lexerStackTop->state;
}

bool lexerIgnoreWhitespace(int *index, token *t) {
	switch (inputString[*index]) {
	case ' ':
	case '\n':
	case '\r':
	case '\t':
	case '\v':
	case '\f':
		// Ignore these whitespace characters
		(*index)++;
		return true;
	}
	return false;
}

void lexerCatchAll(int *index, token *t) {
	switch (inputString[*index]) {
	case '\0':
		// Input has ended
		t->type = CHAR_NUL;
		(*index)++;
		break;
	default:
		// Unexpected characters
		t->type = ERROR;
		t->value = &inputString[*index];
		(*index)++;
	}
}
