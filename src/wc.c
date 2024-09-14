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
#include <math.h>
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

struct align {
	int lines;
	int words;
	int bytes;
	int chars;
};

static struct counts get_counts(FILE *fp);
static void print_counts(struct counts *cnts, struct options *opts, char *name, struct align *max);

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
print_counts(struct counts *cnts, struct options *opts, char *name, struct align *max)
{
	if (opts->l)
		printf("%*zu ", max->lines, cnts->lines);
	if (opts->w)
		printf("%*zu ", max->words, cnts->words);
	if (opts->c)
		printf("%*zu ", max->bytes, cnts->bytes);
	if (opts->m)
		printf("%*zu ", max->chars, cnts->chars);
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

	/* For alignment */
	struct align max = {
		.lines = 0,
		.words = 0,
		.bytes = 0,
		.chars = 0,
	};

	FILE *fp;
	bool useStdin;

	/* Get max lengths */
	for (int i = 0; i < argc; ++i) {
		/* Skip stdin */
		if (strcmp(argv[i], "-") == 0)
			continue;

		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			perror(argv[i]);
			ret = EXIT_FAILURE;
			continue;
		}

		cnts = get_counts(fp);

		total.lines += cnts.lines;
		total.words += cnts.words;
		total.bytes += cnts.bytes;
		total.chars += cnts.chars;

		if (log10(total.lines) > max.lines)
			max.lines = log10(total.lines) + 1;
		else if (log10(cnts.lines) > max.lines)
			max.lines = log10(cnts.lines) + 1;

		if (log10(total.words) > max.words)
			max.words = log10(total.words) + 1;
		else if (log10(cnts.words) > max.words)
			max.words = log10(cnts.words) + 1;

		if (log10(total.bytes) > max.bytes)
			max.bytes = log10(total.bytes) + 1;
		else if (log10(cnts.bytes) > max.bytes)
			max.bytes = log10(cnts.bytes) + 1;

		if (log10(total.chars) > max.chars)
			max.chars = log10(total.chars) + 1;
		else if (log10(cnts.chars) > max.chars)
			max.chars = log10(cnts.chars) + 1;

		fclose(fp);
	}

	if (argc < 1) {
		cnts = get_counts(stdin);
		print_counts(&cnts, &opts, "", &max);
		return EXIT_SUCCESS;
	}

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
		print_counts(&cnts, &opts, argv[i], &max);

		if (!useStdin)
			fclose(fp);
	}

	if (argc > 1)
		print_counts(&total, &opts, "total", &max);

	return ret;
}
