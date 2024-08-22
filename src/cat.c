/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct options {
	bool uflag;
};

static void cat_buffer(FILE *fp);
static void cat_bytes(FILE *fp);

static void
cat_buffer(FILE *fp)
{
	char *buf = NULL;
	size_t len = 0;
	ssize_t nread;

	while ((nread = getline(&buf, &len, fp)) != -1) {
		if (buf[nread - 1] == '\n')
			buf[nread - 1] = '\0';
		puts(buf);
	}

	free(buf);
}

static void
cat_bytes(FILE *fp)
{
	char *buf = NULL;
	size_t len = 0;
	ssize_t nread;

	while ((nread = getline(&buf, &len, fp)) != -1)
		fwrite(buf, 1, nread, stdout);

	free(buf);
}

int
main(int argc, char *argv[])
{
	struct options opts = {
		.uflag = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":u")) != -1;) {
		switch (opt) {
		case 'u':
			opts.uflag = true;
			break;
		case '?':
			fprintf(stderr, "unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	/* Use stdin if no arguments */
	if (argc == 0) {
		if (opts.uflag)
			cat_bytes(stdin);
		else
			cat_buffer(stdin);
		return EXIT_SUCCESS;
	}

	FILE *fp;
	for (int i = 0; i < argc; i++) {
		/* Handle '-' as stdin */
		if (strcmp(argv[i], "-") == 0) {
			if (opts.uflag)
				cat_bytes(stdin);
			else
				cat_buffer(stdin);
			continue;
		}

		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			perror(argv[i]);
			return EXIT_FAILURE;
		}

		if (opts.uflag)
			cat_bytes(fp);
		else
			cat_buffer(fp);

		fclose(fp);
	}

	return EXIT_SUCCESS;
}
