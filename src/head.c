/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct options {
	bool f;
	bool c;
	char *carg;
	bool n;
	char *narg;
};

static bool verify_number(char *num);

static bool
verify_number(char *num)
{
	/* num[0] can be + or - */
	if (num[0] == '+' || num[0] == '-')
		++num;

	for (size_t i = 0; num[i] != '\0'; ++i) {
		if (!isdigit(num[i]))
			return false;
	}

	return true;
}

int
head_main(int argc, char *argv[])
{
	struct options opts = {
		.f = false,
		.c = false,
		.carg = NULL,
		.n = false,
		.narg = NULL,
	};

	for (int opt; (opt = getopt(argc, argv, ":fc:n:")) != -1;) {
		switch (opt) {
		case 'f':
			opts.f = true;
			break;
		case 'c':
			opts.c = true;
			opts.carg = optarg;
			break;
		case 'n':
			opts.n = true;
			opts.narg = optarg;
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

	/* Default options */
	if (!opts.c && !opts.n) {
		opts.n = true;
		opts.narg = "10";
	}

	if (opts.c && !verify_number(opts.carg)) {
		fprintf(stderr, "invalid number of bytes: %s\n", opts.carg);
		return EXIT_FAILURE;
	}

	if (opts.n && !verify_number(opts.narg)) {
		fprintf(stderr, "invalid number of lines: %s\n", opts.narg);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
