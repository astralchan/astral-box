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
	bool Lflag;
	bool Pflag;
};

int
pwd_main(int argc, char *argv[])
{
	struct options opts = {
		.Lflag = true,
		.Pflag = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":LP")) != -1;) {
		switch (opt) {
		case 'L':
			opts.Lflag = true;
			break;
		case 'P':
			opts.Pflag = true;
			break;
		case '?':
			fprintf(stderr, "pwd: unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	/* P overrides L */
	if (opts.Lflag && opts.Pflag)
		opts.Lflag = false;

	char *path = getcwd(NULL, 0);
	if (path == NULL) {
		perror("getcwd");
		return EXIT_FAILURE;
	}

	puts(path);
	free(path);

	return EXIT_SUCCESS;
}
