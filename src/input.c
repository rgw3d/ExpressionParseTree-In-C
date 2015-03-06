#include "input.h"

#include <stdio.h>
#include <string.h>

// Displays a prompt for input and reads it into buf with the size of sz.
int rdLine(const char *prompt, char *buf, size_t sz){
	int ch, extra;

	// Get line with buffer overrun protection.
	if (prompt != NULL) {
		printf ("%s", prompt);
		fflush (stdout);
	}
	if (fgets (buf, sz, stdin) == NULL)
		return -1;

	// If it was too long, there'll be no newline. In that case, we flush
	// to end of line so that excess doesn't affect the next call.
	if (buf[strlen(buf)-1] != '\n') {
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? 1 : 0;
	}

	// Otherwise remove newline and give string back to caller.
	buf[strlen(buf)-1] = '\0';
	return 0;
}
