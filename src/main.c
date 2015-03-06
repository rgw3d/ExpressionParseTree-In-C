#include <stdio.h>
#include <stdlib.h>

#include "stringstack.h"
#include "lexer.h"
#include "input.h"

void init(){
	spInit();
}

// Deallocate all dynamically memory
void cleanup(){
	deinitLexer();
	ssCleanup();
	spCleanup();
}

int main(int argc, const char *argv[]){
	init();

	// I'm lazy and I'm limiting the user's input to 999 characters. :P
	char input[1000];
	int ret = rdLine("> ", input, sizeof(input));

	if(ret == 0){
		// Input not truncated and is not empty
		initLexer(input);

		token t;
		do{
			t = lexerNext();

			// Print info
			printf("Token type: %2d %-20s \t", t.type, tokenTypeName[t.type + 1]);

			if(t.value == NULL){
				printf("Token value: %-20s \t", "NULL");
			} else {
				printf("Token value: %-20s \t", t.value);
			}

			// Wait
			printf("[press enter]");
			getchar();
		} while(t.type != ERROR && t.type != CHAR_NUL);
	}

	cleanup();
	return 0;
}
