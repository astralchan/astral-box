/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct options {
	bool a;
	bool i;
};

int
tee_main(int argc, char *argv[])
{
	struct options opts = {
		.a = false,
		.i = false,
	};

	for (int opt; (opt = getopt(argc, argv, "ai")) != -1;) {
		switch (opt) {
		case 'a':
			opts.a = true;
			break;
		case 'i':
			opts.i = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	if (opts.i)
		signal(SIGINT, SIG_IGN);

	return EXIT_SUCCESS;
}
