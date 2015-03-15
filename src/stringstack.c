#include "stringstack.h"

#include <string.h>
#include "memwrapper.h"

static sPtr *spTop = NULL;
static bool ssCleanedUp = false;

int ssLen(ssChar *top){
	int length = 1;

	ssChar *prev = top->prev;
	while(prev != NULL){
		// While the current node actually exists
		length++;
		prev = prev->prev;
	}

	return length;
}

// Creates a dynamically allocated string. Remember to deallocate stack!
char *ssStr(ssChar *top){
	int len = ssLen(top);
	// Allocate memory
	char *str = (char *)mwalloc(sizeof(char) * (len + 1));

	// Save reference
	spPush(str);

	// Clear existing data
	memset(str, '~', len * sizeof(char));
	str[len] = '\0'; // NUL character marks end of string

	ssChar *prev = top;
	int index = len - 1;
	while(prev != NULL){
		// Insert character into string
		str[index] = prev->c;

		prev = prev->prev;
		index--;
	}

	return str;
}

// Pushes a character and return a pointer to the pushed element.
ssChar *ssPush(char c, ssChar *top){
	ssChar *e = (ssChar *)mwalloc(sizeof(ssChar));

	if(top != NULL){
		e->prev = top;
	} else {
		e->prev = NULL;
	}

	// Set value
	e->c = c;

	return e;
}

// Pops and return the pointer to the new top element
ssChar *ssPop(ssChar *top){
	ssChar *prev = top->prev;

	// Free memory
	mwfree(top, sizeof(ssChar));

	return prev;
}

// Gets pointer to the root element
ssChar *ssRoot(ssChar *top){
	ssChar *prev = top->prev;

	while(prev->prev != NULL){
		// While the current node is not the root node, get the previous one.
		prev = prev->prev;
	}

	return prev;
}

// Destroys the stack
void ssDestroy(ssChar *top){
	ssChar *prev = top;

	while(prev != NULL){
		// While the current node exists
		prev = ssPop(prev);
	}
}

// Pushes a character and return a pointer to the pushed element.
void spPush(char *p){
	// Set to false if something has been pushed on the stack
	ssCleanedUp = false;
	sPtr *e = (sPtr *)mwalloc(sizeof(sPtr));

	e->prev = spTop;

	// Set value
	e->p = p;

	// Update new top
	spTop = e;
}

// Pop stack
void spPop(){
	sPtr *prev = spTop->prev;

	// Free memory
	mwfree(spTop, sizeof(sPtr));

	// Update new top
	spTop = prev;
}

// Deallocates and destroys the stack
void spDestroy(){
	if(!ssCleanedUp){
		// Must deallocate strings first
		ssCleanup();
	}

	while(spTop != NULL){
		spPop();
	}
}

// Initializes the stack
void spInit(){
	spTop = (sPtr *)mwalloc(sizeof(sPtr));
	spTop->prev = NULL;
	spTop->p = NULL;
}

void spCleanup(){
	spDestroy();
}

// Deallocates all dynamically allocated strings allocated by ssStr
void ssCleanup(){
	sPtr *prev = spTop;

	// While the current element exist
	while(prev != NULL){
		// Free memory if the pointer actually points to something
		if(prev->p != NULL){
			mwfree(prev->p, sizeof(char) * (strlen(prev->p) + 1));
		}

		prev = prev->prev;
	}

	ssCleanedUp = true;
}
