#include "token.h"
#include <stdlib.h>

void tokenCopy(token *src, token *dest){
	dest->type = src->type;
	dest->value = src->value;
}
