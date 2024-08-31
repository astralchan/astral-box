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
	bool _1;
	bool _2;
	bool _3;
};

int
comm_main(int argc, char *argv[])
{
	struct options opts = {
		._1 = false,
		._2 = false,
		._3 = false,
	};

	for (int opt; (opt = getopt(argc, argv, "123")) != -1;) {
		switch (opt) {
		case '1':
			opts._1 = true;
			break;
		case '2':
			opts._2 = true;
			break;
		case '3':
			opts._3 = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	return EXIT_SUCCESS;
}
