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
			break;
		case 'P':
			opts.P = true;
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

	if (opts.L && opts.P)
		opts.L = false;

	return EXIT_SUCCESS;
}
