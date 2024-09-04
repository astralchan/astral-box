/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

struct options {
	bool L;
	bool P;
};

int
pwd_main(int argc, char *argv[])
{
	struct options opts = {
		.L = false,
		.P = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":LP")) != -1;) {
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

	(void)argc;
	(void)argv;

	if (!opts.L && !opts.P)
		opts.L = true;

	if (opts.L && opts.P)
		opts.L = false;

	char *pwd, *cwd;

	if (opts.L) {
		pwd = getenv("PWD");
		if (pwd == NULL) {
			fputs("PWD not set\n", stderr);
			return EXIT_FAILURE;
		}

		/* If there is dot or dot-dot, specify -P */
		char tmp[PATH_MAX];
		strncpy(tmp, pwd, sizeof(tmp));
		tmp[sizeof(tmp) - 1] = '\0';

		for (char *tok = strtok(tmp, "/"); tok; tok = strtok(NULL, "/")) {
			if (strcmp(tok, ".") == 0 || strcmp(tok, "..") == 0) {
				opts.P = true;
				break;
			}
		}

		/* Unspecified in POSIX */
		if (strlen(pwd) + 1 > PATH_MAX)
			opts.P = true;

		opts.L = !opts.P;
	}

	if (opts.L) {
		(void)puts(pwd);
		return EXIT_SUCCESS;
	}

	if (opts.P) {
		char buf[PATH_MAX];
		cwd = getcwd(buf, sizeof(buf));
		if (cwd == NULL) {
			perror("getcwd");
			return EXIT_FAILURE;
		}
		(void)puts(cwd);
	}

	return EXIT_SUCCESS;
}
