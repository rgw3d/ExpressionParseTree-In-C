#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "InputSanitation.h"
#include "InputSanitation.c"
#define TRUE 1
#define FALSE 0


int main(){


	while(TRUE){
		printf("\n");
		printf("\nType in an expression to simplify\n");
		
		char input[100];
		scanf(" %[^\n]s",input);

		char * expression = (char *) malloc(sizeof(char) * (strlen(input)+1));
		memcpy(expression,&input[0],strlen(input)+1);
		
		//input = (char *) realloc(input,sizeof(char) * strlen(input));
		
		if(strncmp(expression, "break",5) ==0){
			free(expression);
			break;
		}
		if(invalidExpression(expression)){
			free(expression);
			continue;
		}
		printf("\tExpression was accepted \n");
		
		expression = handSanitizer(expression);
		free(expression);
	}

	return 1;
	
}





