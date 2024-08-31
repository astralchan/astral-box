/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct options {
	bool b;
	char *barg;
	bool c;
	char *carg;
	bool d;
	char *darg;
	bool f;
	char *farg;
	bool n;
	bool s;
};

static bool verify_list(const char *list);

static bool
verify_list(const char *list)
{
	if (list == NULL)
		return false;

	bool inSep, inRange;
	inSep = inRange = false;
	for (const char *p = list; *p != '\0'; ++p) {
		if (inRange && *p == '-') {
			fprintf(stderr, "invalid range\n");
			return false;
		}

		if (!inRange && *p == '-') {
			inRange = true;
			continue;
		}

		if (!inSep && (*p == ',' || isspace(*p))) {
			inSep = true;
			continue;
		}

		if (!inSep && !inRange && !isdigit(*p)) {
			fprintf(stderr, "invalid number in list\n");
			return false;
		}
	}

	return true;
}

int
cut_main(int argc, char *argv[])
{
	struct options opts = {
		.b = false,
		.c = false,
		.d = false,
		.f = false,
		.n = false,
		.s = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":b:c:d:f:ns")) != -1;) {
		switch (opt) {
		case 'b':
			opts.b = true;
			opts.barg = optarg;
			break;
		case 'c':
			opts.c = true;
			opts.carg = optarg;
			break;
		case 'd':
			opts.d = true;
			opts.darg = optarg;
			break;
		case 'f':
			opts.f = true;
			opts.farg = optarg;
			break;
		case 'n':
			opts.n = true;
			break;
		case 's':
			opts.s = true;
			break;
		case ':':
			fprintf(stderr, "option: -%c requires an argument\n", optopt);
			return EXIT_FAILURE;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	if (opts.f) {
		if (!verify_list(opts.farg))
			return EXIT_FAILURE;
		puts("Check passed");
	}

	return EXIT_SUCCESS;
}
