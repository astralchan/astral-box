/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

struct options {
	bool l;
	bool w;
	bool c;
	bool m;
};

struct counts {
	size_t lines;
	size_t words;
	size_t bytes;
	size_t chars;
};

static struct counts get_counts(FILE *fp);
static void print_counts(struct counts *cnts, struct options *opts, char *name);

static struct counts
get_counts(FILE *fp)
{
	struct counts ret = {
		.lines = 0,
		.words = 0,
		.bytes = 0,
		.chars = 0,
	};

	char buf[MB_LEN_MAX];
	size_t bufLen = 0;

	bool inWord, inSpace;
	inWord = inSpace = false;
	for (wint_t wc; (wc = fgetwc(fp)) != WEOF; ++ret.chars) {
		inSpace = iswspace(wc);

		if (inSpace && inWord) {
			inWord = false;
			++ret.words;
		}
		if (!inSpace && !inWord)
			inWord = true;

		if (wc == L'\n')
			++ret.lines;

		/* Count bytes from characters */
		bufLen = wcrtomb(buf, wc, NULL);
		ret.bytes += (bufLen == (size_t)-1) ? 0 : bufLen;
	}

	return ret;
}

static void
print_counts(struct counts *cnts, struct options *opts, char *name)
{
	if (opts->l)
		printf("%zu ", cnts->lines);
	if (opts->w)
		printf("%zu ", cnts->words);
	if (opts->c)
		printf("%zu ", cnts->bytes);
	if (opts->m)
		printf("%zu ", cnts->chars);
	puts(name);
}

int
wc_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct options opts = {
		.l = false,
		.w = false,
		.c = false,
		.m = false,
	};

	setlocale(LC_ALL, "");

	for (int opt; (opt = getopt(argc, argv, ":lwcm")) != -1;) {
		switch (opt) {
		case 'l':
			opts.l = true;
			break;
		case 'w':
			opts.w = true;
			break;
		case 'c':
			opts.c = true;
			break;
		case 'm':
			opts.m = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	/* Default flags if no options specified */
	if (!opts.l && !opts.w && !opts.c && !opts.m)
		opts.l = opts.w = opts.c = true;

	argc -= optind;
	argv += optind;

	struct counts total = {
		.lines = 0,
		.words = 0,
		.bytes = 0,
		.chars = 0,
	};

	struct counts cnts;

	if (argc < 1) {
		cnts = get_counts(stdin);
		print_counts(&cnts, &opts, "-");
		return EXIT_SUCCESS;
	}

	FILE *fp;
	bool useStdin;
	for (int i = 0; i < argc; ++i) {
		useStdin = strcmp(argv[i], "-") == 0;

		if (!useStdin) {
			fp = fopen(argv[i], "r");
			if (fp == NULL) {
				perror(argv[i]);
				ret = EXIT_FAILURE;
				continue;
			}
		}

		if (useStdin)
			fp = stdin;

		cnts = get_counts(fp);
		print_counts(&cnts, &opts, argv[i]);

		total.lines += cnts.lines;
		total.words += cnts.words;
		total.bytes += cnts.bytes;
		total.chars += cnts.chars;

		if (!useStdin)
			fclose(fp);
	}

	if (argc > 1)
		print_counts(&total, &opts, "total");

	return ret;
}
