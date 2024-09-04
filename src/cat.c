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
	bool u;
};

static void cat_buffer(FILE *fp);
static void cat_bytes(FILE *fp);

static void
cat_buffer(FILE *fp)
{
	char *buf = NULL;
	size_t len = 0;
	ssize_t nread;

	while ((nread = getline(&buf, &len, fp)) != -1)
		fputs(buf, stdout);

	free(buf);
}

static void
cat_bytes(FILE *fp)
{
	for (int c; (c = fgetc(fp)) != EOF;)
		putchar(c);
}

int
cat_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct options opts = {
		.u = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":u")) != -1;) {
		switch (opt) {
		case 'u':
			opts.u = true;
			break;
		case '?':
			fprintf(stderr, "unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	/* Use stdin if no args given */
	if (argc < 1) {
		if (opts.u)
			cat_bytes(stdin);
		else
			cat_buffer(stdin);
		return EXIT_SUCCESS;
	}

	FILE *fp;
	bool useStdin = false;
	for (int i = 0; i < argc; ++i) {
		useStdin = strcmp(argv[i], "-") == 0;

		if (!useStdin) {
			fp = fopen(argv[i], "r");
			if (!fp) {
				perror(argv[i]);
				ret = EXIT_FAILURE;
				continue;
			}
		}

		if (useStdin)
			fp = stdin;

		if (opts.u)
			cat_bytes(fp);
		else
			cat_buffer(fp);

		if (!useStdin)
			fclose(fp);
	}

	return ret;
}
