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
	bool t;
	char *targ;
};

static bool verify_tabstop(char *targ);
static void expand_file(FILE *fp, struct options *opts);

static bool
verify_tabstop(char *targ)
{
	if (targ == NULL)
		return false;

	/* check if strtok returns null for comma or space */
	char *tok;

	if ((tok = strtok(targ, ",")) == NULL)
		return false;

	if ((tok = strtok(targ, " ")) == NULL)
		return false;

	return true;
}

static void
expand_file(FILE *fp, struct options *opts)
{
}

int
expand_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct options opts = {
		.t = false,
		.targ = NULL,
	};

	for (int opt; (opt = getopt(argc, argv, ":t:")) != -1;) {
		switch (opt) {
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

	if (!opts.t)
		opts.targ = "8";

	argc -= optind;
	argv += optind;

	FILE *fp;

	if (argc < 1) {
		expand_file(stdin, &opts);
		return EXIT_SUCCESS;
	}

	for (int i = 0; i < argc; ++i) {
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			perror(argv[i]);
			ret = EXIT_FAILURE;
			continue;
		}
		expand_file(fp, &opts);
		fclose(fp);
	}

	return ret;
}
