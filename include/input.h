#pragma once

#include <stddef.h>

/**
 * @brief Displays a prompt for input and reads it into buf with the size of sz.
 *
 * A read line function that displays a prompt and reads only up to the size of
 * the buffer. Everything that exceeds the size of the buffer is discarded.
 * Provides a safer alternative than scanf("%s") that could potentially cause
 * a SIGSEGV.
 *
 * @param prompt a pointer to a string containing the prompt to display on
 * 				 screen.
 * @param buf a pointer to the buffer that the input will be saved to.
 * @param size the size of the buffer.
 * @return 0 on success, -1 if no input was received, and 1 if input exceeds
 * 		   the size of the buffer.
 */
int rdLine(const char *prompt, char *buf, size_t size);
