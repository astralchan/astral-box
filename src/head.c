/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct options {
	int num;
};

int
head_main(int argc, char *argv[])
{
	struct options opts = {
		.num = 10,
	};

	for (int opt; (opt = getopt(argc, argv, ":n:")) != -1;) {
		switch (opt) {
		case 'n':
			opts.num = atoi(optarg);
			break;
		case ':':
			fprintf(stderr, "option -%c requires an operand\n", optopt);
			return EXIT_FAILURE;
		default:
			fprintf(stderr, "Usage: head [-n num] [file]\n");
			return EXIT_FAILURE;
		}
	}

	printf("num: %d\n", opts.num);

	return EXIT_SUCCESS;
}
