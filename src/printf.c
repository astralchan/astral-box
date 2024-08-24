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

struct state {
	bool inFmt;
	bool inEsc;
	char val;
};

int
printf_main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s fmt [arg]...\n", argv[0]);
		return EXIT_FAILURE;
	}

	--argc;
	++argv;

	struct state s = {
		.inFmt = false,
		.inEsc = false,
		.val = '\0',
	};
	const char *fmt = argv[0];

	int index = 1;
	for (const char *p = fmt; *p; ++p) {
		s.val = *p;

		if (!s.inEsc &&
		    !s.inFmt &&
		    s.val != '%' &&
		    s.val != '\\') {
			putchar(s.val);
			continue;
		}

		if (s.inFmt && s.val == '%') {
			putchar('%');
			s.inFmt = false;
			continue;
		}

		if (!s.inFmt && s.val == '%') {
			s.inFmt = true;
			continue;
		}

		if (!s.inEsc && s.val == '\\') {
			s.inEsc = true;
			continue;
		}

		if (s.inEsc) {
			switch (s.val) {
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

			s.inEsc = false;
			continue;
		}

		/*
		 * TODO
		 * - implement more format specifiers
		 * - verify input
		 */
		if (s.inFmt) {
			if (index >= argc) {
				fprintf(stderr, "printf: not enough arguments\n");
				return EXIT_FAILURE;
			}

			switch (s.val) {
			case 's':
				printf("%s", argv[index++]);
				break;
			case 'i':
			case 'd':
				printf("%d", atoi(argv[index++]));
				break;
			case 'l':
				switch (*++p) {
				case 'd':
					printf("%ld", atol(argv[index++]));
					break;
				default:
					fprintf(stderr, "printf: unknown format specifier: %c\n", *p);
					return EXIT_FAILURE;
				}
				break;
			default:
				fprintf(stderr, "printf: unknown format specifier: %c\n", *p);
				return EXIT_FAILURE;
			}

			s.inFmt = false;
			continue;
		}
	}

	return EXIT_SUCCESS;
}
