/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int
printf_main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s fmt [arg]...\n", argv[0]);
		return EXIT_FAILURE;
	}

	--argc;
	++argv;

	bool inFmt, inEsc;
	inFmt = inEsc = false;
	const char *fmt = argv[0];

	int index = 1;
	for (const char *p = fmt; *p; ++p) {
		if (!inEsc && !inFmt && *p != '%' && *p != '\\') {
			putchar(*p);
			continue;
		}

		if (!inFmt && *p == '%') {
			inFmt = true;
			continue;
		}

		if (!inEsc && *p == '\\') {
			inEsc = true;
			continue;
		}

		if (inEsc) {
			switch (*p) {
			case 'n':
				putchar('\n');
				break;
			case 't':
				putchar('\t');
				break;
			case 'r':
				putchar('\r');
				break;
			case 'v':
				putchar('\v');
				break;
			case 'f':
				putchar('\f');
				break;
			case 'a':
				putchar('\a');
				break;
			case 'b':
				putchar('\b');
				break;
			case '\\':
				putchar('\\');
				break;
			case '\'':
				putchar('\'');
				break;
			case '\"':
				putchar('\"');
				break;
			case '!':
				putchar('!');
				break;
			case '?':
				putchar('?');
				break;
			default:
				putchar('\\');
				putchar(*p);
				break;
			}

			inEsc = false;
			continue;
		}

		/*
		 * TODO
		 * - implement more format specifiers
		 * - verify input
		 */
		if (inFmt) {
			switch (*p) {
			case 's':
				printf("%s", argv[index++]);
				break;
			case 'i':
			case 'd':
				printf("%d", atoi(argv[index++]));
				break;
			default:
				fprintf(stderr, "printf: unknown format specifier: %c\n", *p);
				return EXIT_FAILURE;
			}

			inFmt = false;
			continue;
		}
	}

	return EXIT_SUCCESS;
}
