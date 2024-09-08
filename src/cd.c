/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct options {
	bool L;
	bool P;
};

int
cd_main(int argc, char *argv[])
{
	struct options opts = {
		.L = false,
		.P = false,
	};

	for (int opt; (opt = getopt(argc, argv, "LP")) != -1;) {
		switch (opt) {
		case 'L':
			opts.L = true;
			opts.P = false;
			break;
		case 'P':
			opts.P = true;
			opts.L = false;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	if (!opts.L && !opts.P)
		opts.L = true;

	if (argc < 1 && getenv("HOME") == NULL) {
		fprintf(stderr, "HOME not set\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
