#ifndef INPUT_SANITATION_INCLUDED
#define INPUT_SANITATION_INCLUDED
/* ^^ these are the include guards */

int invalidExpression(char * input);
char * handSanitizer(char *x);
char * replaceString( const char *toReplace, const char * replace, char * equation);

#endif
