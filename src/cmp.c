/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct options {
	bool l;
	bool s;
};

static void close_files(FILE *f1, FILE *f2);

static void
close_files(FILE *f1, FILE *f2)
{
	if (f1 != stdin)
		fclose(f1);
	if (f2 != stdin)
		fclose(f2);
}

int
cmp_main(int argc, char *argv[])
{
	struct options opts = {
		.l = false,
		.s = false,
	};

	for (int opt; (opt = getopt(argc, argv, "ls")) != -1;) {
		switch (opt) {
		case 'l':
			opts.l = true;
			break;
		case 's':
			opts.s = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	if (opts.l && opts.s) {
		fprintf(stderr, "cannot specify both -l and -s\n");
		return EXIT_FAILURE;
	}

	FILE *f1, *f2;
	switch (argc) {
	case 0:
		fprintf(stderr, "missing operand\n");
		return EXIT_FAILURE;
	case 1:
		/* Written "-" and implicit stdin are the same */
		if (strcmp(argv[0], "-") == 0)
			return 0;

		f1 = fopen(argv[0], "r");
		if (f1 == NULL) {
			perror(argv[0]);
			return EXIT_FAILURE;
		}
		f2 = stdin;
		break;
	case 2:
		f1 = (strcmp(argv[0], "-") == 0) ? stdin : fopen(argv[0], "r");
		if (f1 == NULL) {
			perror(argv[0]);
			return EXIT_FAILURE;
		}
		f2 = (strcmp(argv[1], "-") == 0) ? stdin : fopen(argv[1], "r");
		if (f2 == NULL) {
			perror(argv[1]);
			fclose(f1);
			return EXIT_FAILURE;
		}
		break;
	}

	/* If same file is specified return no difference */
	if (f1 == f2) {
		fclose(f1);
		fclose(f2);
		return 0;
	}

	/* Compare byte-by-byte */
	for (int c1, c2; (c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF;) {
		if (c1 != c2) {
			close_files(f1, f2);
			return 1;
		}
		return 0;
	}

	close_files(f1, f2);

	return EXIT_SUCCESS;
}
