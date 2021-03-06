#include "InputSanitation.h"
#define TRUE 1
#define FALSE 0

int invalidExpression(char * input){

	size_t len = strlen(input);
	
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
	int i;
	for(i =0; i<len; i++){
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
char * handSanitizer(char *fix) {//returns a string that is dynamically alcolated.
		fix = replaceString("--","+",fix);
		fix = replaceString(" ","",fix);
		fix = replaceString("-", "+-",fix);
		fix = replaceString("X", "x",fix);
		fix = replaceString("^+-", "^-",fix);
		fix = replaceString("++-", "+-", fix);
		fix = replaceString("*+-", "*-",fix);
		fix = replaceString("(+-", "(-",fix);
		fix = replaceString(")(",")*(",fix);
		
		if(strncmp(fix,"+-",2)==0){
			size_t len = strlen(fix);//original length
			memcpy(fix,&fix[1],len);//cpy to fix starting at fix[1], for orig len-1
			fix = (char *) realloc(fix,len);//realloc space to be smaller
		}
		if(strncmp(fix,"+",1)==0){//same as above
			size_t len = strlen(fix);
			memcpy(fix,&fix[1],len);
			fix = (char *) realloc(fix,len);
		}
	
		printf("Reformatted Equation: %s",fix);
		return fix;
		
}

char * replaceString( const char *toReplace, const char * replace, char * equation){
	size_t len = strlen(toReplace);
	int i;
	for(i= 0; i<strlen(equation); i++){
		
		char * compareTo = (char *) malloc(sizeof(char) * (len +1));
		
		//(i>=len) ? memcpy(compareTo,&equation[i-len], len): memcpy(compareTo, &equation[i],len);
		if(i>=len)//prevents invalid read 
			memcpy(compareTo,&equation[i-len],len);
		else{
			free(compareTo);
			continue;
		}

			
		compareTo[len]='\0';//add that null byte
		if(strncmp(compareTo,toReplace,len)==0){
			char * result = (char *) malloc((strlen(equation)-len+strlen(replace)+1) * sizeof(char));
			memcpy( result, equation, i-len);
			memcpy( &result[i-len], replace, strlen(replace)); 
			memcpy( &result[i-len+strlen(replace)], &equation[i], strlen(equation)-i);
			result[strlen(equation)-len+strlen(replace)] = '\0';
			equation = (char *) realloc(equation,strlen(result)+1);
			memcpy(equation,result,strlen(result)+1);
			printf("\tNew Equation: %s\n",equation);
			i= i+strlen(replace);
			free(result);
		}
		free(compareTo);
	}
	return equation;
}
