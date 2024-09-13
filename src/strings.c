/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct options {
	bool a;
	bool n;
	char *narg;
	bool t;
	char *targ;
};

static bool verify_number(char *number);
static bool verify_radix(char *radix);
static void strings_file(FILE *fp, struct options *opts);

static bool
verify_number(char *number)
{
	/* Plus sign allowed */
	if (number[0] == '+')
		++number;

	for (size_t i = 0; i < strlen(number); ++i) {
		if (!isdigit(number[i])) {
			return false;
		}
	}

	return true;
}

static bool
verify_radix(char *radix)
{
	if (strlen(radix) != 1) {
		return false;
	}

	if (strcmp(radix, "d") && strcmp(radix, "o") && strcmp(radix, "x"))
		return false;

	return true;
}

static void
strings_file(FILE *fp, struct options *opts)
{
	bool inPrint, inLoc;
	inPrint = inLoc = false;
	long loc = 0;
	char seq[PIPE_BUF];
	int seqIndex = 0;
	int printableCount = 0;
	int minLen = atoi(opts->narg);

	for (int c; (c = fgetc(fp)) != EOF;) {
		if (!inPrint && isprint(c)) {
			inPrint = true;
			loc = ftell(fp) - 2;

			if (opts->t && !inLoc) {
				inLoc = true;
				printf(
				    (opts->targ[0] == 'd') ? "%ld " :
				    (opts->targ[0] == 'o') ? "%lo " : "%lx ",
				    loc
				);
				putchar(' ');
			}

			seq[seqIndex++] = c;
			++printableCount;
			continue;
		}

		if (inPrint && isprint(c)) {
			if (seqIndex < sizeof(seq) - 1) {
				seq[seqIndex++] = c;
				++printableCount;
			}
			continue;
		}

		if (inPrint && !isprint(c)) {
			inPrint = false;
			inLoc = false;

			seq[seqIndex] = '\0';

			if (printableCount >= minLen)
				puts(seq);

			seqIndex = 0;
			printableCount = 0;
			continue;
		}
	}

	/* EOF in printable sequence */
	if (inPrint) {
		seq[seqIndex] = '\0';
		if (printableCount >= minLen)
			puts(seq);
	}
}

int
strings_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct options opts = {
		.a = false,
		.n = false,
		.narg = NULL,
		.t = false,
		.targ = NULL,
	};

	for (int opt; (opt = getopt(argc, argv, ":an:t:")) != -1;) {
		switch (opt) {
		case 'a':
			opts.a = true;
			break;
		case 'n':
			opts.n = true;
			opts.narg = optarg;
			break;
		case 't':
			opts.t = true;
			opts.targ = optarg;
			break;
		case ':':
			fprintf(stderr, "option -%c requires an argument\n", optopt);
			return EXIT_FAILURE;
		case '?':
			fprintf(stderr, "unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	/* Default n is 4 */
	if (!opts.n) {
		opts.n = true;
		opts.narg = "4";
	}

	if (!verify_number(opts.narg)) {
		fprintf(stderr, "invalid number: %s\n", opts.narg);
		return EXIT_FAILURE;
	}

	if (opts.t) {
		if (!verify_radix(opts.targ)) {
			fprintf(stderr, "invalid radix: %s\n", opts.targ);
			return EXIT_FAILURE;
		}
	}

	if (argc < 1) {
		strings_file(stdin, &opts);
		return EXIT_SUCCESS;
	}

	FILE *fp;
	for (int i = 0; i < argc; ++i) {
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			perror(argv[i]);
			ret = EXIT_FAILURE;
			continue;
		}

		strings_file(fp, &opts);

		fclose(fp);
	}

	return ret;
}
