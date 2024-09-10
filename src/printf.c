/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int
printf_main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "no format string\n");
		return EXIT_FAILURE;
	}

	--argc;
	++argv;

	bool inEscape, inFormat;
	inEscape = inFormat = false;
	int index = 1;
	for (const char *p = argv[0]; *p; ++p) {
		if (!inEscape && !inFormat && *p != '%' && *p != '\\') {
			putchar(*p);
			continue;
		}

		if (!inFormat && *p == '%') {
			inFormat = true;
			continue;
		}

		if (!inEscape && *p == '\\') {
			inEscape = true;
			continue;
		}

		/* Can't put in switch as it will check for index */
		if (inFormat && *p == '%') {
			putchar('%');
			inFormat = false;
			continue;
		}

		if (inEscape) {
			switch (*p) {
			case 'n':
				putchar('\n');
				break;
			case 't':
				putchar('\t');
				break;
			case '\\':
				putchar('\\');
				break;
			case '!':
				putchar('!');
				break;
			default:
				putchar('\\');
				putchar(*p);
			}

			inEscape = false;
			continue;
		}

		if (inFormat) {
			if (index >= argc) {
				fprintf(stderr, "not enough arguments\n");
				return EXIT_FAILURE;
			}

			switch (*p) {
			case 's':
				printf("%s", argv[index++]);
				break;
			case 'i':
				printf("%d", atoi(argv[index++]));
				break;
			case 'd':
				printf("%d", atoi(argv[index++]));
				break;
			case 'c':
				printf("%c", *argv[index++]);
				break;
			case 'l':
				switch (*++p) {
				case 'd':
					printf("%ld", atol(argv[index++]));
					break;
				default:
					fprintf(stderr, "unknown format specifier: l%c\n", *p);
					return EXIT_FAILURE;
				}
			default:
				fprintf(stderr, "unknown format specifier: %c\n", *p);
				return EXIT_FAILURE;
			}

			inFormat = false;
			continue;
		}
	}

	return EXIT_SUCCESS;
}
