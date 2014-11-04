#include <stdio.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

int invalidExpression(char expression[100]);
char * handSanitizer(char *x);
char * replace( const char *toReplace, const char * replace, char * equation);

void main(){

	while(TRUE){
		printf("\nType in an expression to simplify\n");
		
		//char * name[1] = "wow";
		
		char input[100];
		scanf(" %[^\n]s",input);
		
		if(strncmp(input, "break",5) ==0){
			break;
		}
		if(invalidExpression(input)){
			continue;
		}
		printf("\tExpression was accepted \n");
		
		char* expression = handSanitizer(input);
		free(expression);
		
		//printf("size of input: %d\n",sizeof(input));
		//printf(expression);
		//printf("size of expression: %d\n",strlen(expression));
	}
	
}

int invalidExpression(char input[100]){

	int len = strlen(input);
	
	if(len<3){
		return TRUE;
	}
	char * equalsSign = strchr(input, '=');
	if(equalsSign != NULL){
		printf("Syntax Error: To short\n");
		return TRUE;
	}
	char * multSign = strchr(input,'*');
	char * addSign = strchr(input,'+');
	char * minSign = strchr(input,'-');
	char * divSign = strchr(input,'/');
	if(!(multSign !=NULL || addSign !=NULL || minSign!=NULL || divSign!=NULL)){
		printf("Syntax Error: Does not contain an opperator\n");
		return TRUE;
	}
	
	multSign = strrchr(input,'*');
	addSign = strrchr(input, '+');
	minSign = strrchr(input, '-');
	divSign = strrchr(input, '/');
	char * endChar = &(input[len-1]);
	if(multSign == endChar || addSign == endChar || minSign == endChar || divSign ==endChar){
		printf("Syntax Error: Ends with opperator\n");
		return TRUE;
	}
	
	char * startChar = &(input[0]);
	if(multSign ==startChar || addSign == startChar || divSign == startChar){
		printf("Syntax Error: Starts with invalid opperator\n");
		return TRUE;
	}
	
	//strpbrk
	char invalidCharacters[] = "!\"#$%&\',:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWYZ[\\]_`~abcdefghijlmnopqrstuvwyz|{}\t\?";
	char * invalCharTest= strpbrk(input,invalidCharacters);
	if(invalCharTest !=NULL){
		printf("Syntax Error: Invalid Character: %c\n",*invalCharTest);
		return TRUE;
	}
	
	
	int openCount =0;
	int closedCount = 0;
	for(int i =0; i<len; i++){
		if(strncmp(&input[i], "(",1)==0){
		openCount++;
		}
		if(strncmp(&input[i], ")",1) ==0){
		closedCount++;
		}
	}
	if(closedCount !=openCount){
		printf("Syntax Error: Uneven amount of Parenthesis\n");
		printf("\tOpen Parenthesis: %d\n", openCount);
		printf("\tClosed Parenthesis: %d\n", closedCount);
		return TRUE;
	}
	
	return FALSE;//good syntax
	
}
char * handSanitizer(char *x) {//returns a string that is dynamically alcolated.  
		char * fix = (char *) malloc(sizeof(char)* (strlen(x)+1));
		strcpy(fix,x);
		fix = replace(" ","",fix);
		fix = replace("--","+",fix);
		fix = replace("-", "+-",fix);
		fix = replace("X", "x",fix);
		fix = replace("^+-", "^-",fix);
		fix = replace("*+-", "*-",fix);
		fix = replace("(+-", "(-",fix);
		fix = replace(")(",")*(",fix);
		//memcpy(fix,fix,strlen(fix));
		if(strncmp(fix,"+-",2)==0){
			size_t len = strlen(fix);
			memcpy(fix,&fix[1],len-1);
			fix = (char *) realloc(fix,len-1);
			fix[len-1]='\0';
		}
		printf("fix address: %d\n",&fix);
		if(strncmp(fix,"+",1)==0){
			size_t len = strlen(fix);
			memcpy(fix,&fix[1],len-1);
			fix = (char *) realloc(fix,len-1);
			fix[len-1]='\0';
		}
		printf("fix address: %d\n",&fix);
		
		
		//
	
		printf("Reformatted Equation: %s",fix);
		return fix;
		
		
		
}

char * replace( const char *toReplace, const char * replace, char * equation){
	size_t len = strlen(toReplace);
	for(int i= len-1; i<strlen(equation); i++){
	
		char blank[100];
		char * compareTo = strncpy(blank,&equation[i-len],len);
		compareTo[len]='\0';//add that null byte
		
		/*printf("\n");
		printf(compareTo);
		printf("\nan int: %d\n",strncmp(compareTo,toReplace,len));
		printf("\n");
		*/
		if(strncmp(compareTo,toReplace,len)==0){
			printf("it got here\n");
			//if toReplace is the same this substring of equation
			//if true, then make a new string with the replaced value.
			char * leftSide = (char *) malloc(((strlen(equation)-len+strlen(replace)+1) * sizeof(char)));
			memcpy( leftSide, &equation[0], i-len);//get left side and put it into leftSide
			leftSide[i-len] = '\0';
			//make sure that it has enough memory to hold the size of the whole thing
			
			printf("\tLeft Side: %s\n",leftSide);
			
			char * rightSide = (char *) malloc((strlen(equation)-i+1) * sizeof(char));
			memcpy( rightSide, &equation[i], strlen(equation)-i);//get right side
			rightSide[strlen(equation)-i]='\0';
			printf("\tRight Side: %s\n",rightSide);
			
			char * newString = strcat(strcat(leftSide,replace),rightSide);
			printf("\tNew String: %s\n",newString);
			/*printf("\tLeftSide strlen: %d\n",i-len);
			printf("\tRightSide strlen: %d\n",strlen(rightSide));
			printf("\tReplace strlen: %d\n",strlen(replace));
			printf("\tthe index being replaced with null character: %d\n",i-len+strlen(rightSide)+strlen(replace));
			*/
			newString[i-len+strlen(rightSide)+strlen(replace)] = '\0';//add the null byte
			
			equation = (char *) realloc(equation,strlen(newString));
			printf("\tOld Equation: %s\n",equation);
			memcpy(&equation[0], &newString[0],strlen(newString));
			equation[i-len+strlen(rightSide)+strlen(replace)] = '\0';
			//equation = newString;
			printf("\tNew Equation: %s\n",equation);
			i = i-len+strlen(replace);//continue looping with this new indx
			free(leftSide);
			free(rightSide);
		}
		//free(compareTo);
	}
	return equation;
}



