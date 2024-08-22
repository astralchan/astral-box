/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

struct options {
	bool lflag;
	bool wflag;
	bool cflag;
	bool mflag;
};

struct counts {
	size_t lines;
	size_t words;
	size_t chars;
};

static struct counts count_chars(FILE *fp);
static struct counts count_bytes(FILE *fp);
static void print_counts(struct counts *cnts, struct options *opts);

static struct counts
count_chars(FILE *fp)
{
	bool inWord, isSpace;
	inWord = isSpace = false;
	struct counts ret = {
		.lines = 0,
		.words = 0,
		.chars = 0,
	};

	for (wint_t wc; (wc = fgetwc(fp)) != WEOF; ++ret.chars) {
		isSpace = iswspace(wc);
		if (isSpace && inWord) {
			++ret.words;
			inWord = false;
		}
		if (!isSpace && !inWord)
			inWord = true;
		if (wc == L'\n')
			++ret.lines;
	}

	return ret;
}

static struct counts
count_bytes(FILE *fp)
{
	bool inWord, isSpace;
	inWord = isSpace = false;
	struct counts ret = {
		.lines = 0,
		.words = 0,
		.chars = 0,
	};

	for (int c; (c = fgetc(fp)) != EOF; ++ret.chars) {
		isSpace = isspace(c);
		if (isSpace && inWord) {
			++ret.words;
			inWord = false;
		}
		if (!isSpace && !inWord)
			inWord = true;
		if (c == '\n')
			++ret.lines;
	}

	return ret;
}

static void
print_counts(struct counts *cnts, struct options *opts)
{
	if (opts->lflag)
		printf("%zu ", cnts->lines);
	if (opts->wflag)
		printf("%zu ", cnts->words);
	if (opts->cflag || opts->mflag)
		printf("%zu ", cnts->chars);
}

int
main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	FILE *fp;
	struct options opts = {
		.lflag = false,
		.wflag = false,
		.cflag = false,
		.mflag = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":lwcm")) != -1;) {
		switch (opt) {
		case 'l':
			opts.lflag = true;
			break;
		case 'w':
			opts.wflag = true;
			break;
		case 'c':
			opts.cflag = true;
			break;
		case 'm':
			opts.mflag = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	if (!opts.lflag && !opts.wflag && !opts.cflag && !opts.mflag) {
		opts.lflag = true;
		opts.wflag = true;
		opts.cflag = true;
	}

	if (opts.cflag && opts.mflag)
		opts.mflag = false;

	argc -= optind;
	argv += optind;

	struct counts totals = {
		.lines = 0,
		.words = 0,
		.chars = 0,
	};

	if (argc == 0) {
		totals = (opts.mflag)
		    ? count_chars(stdin)
		    : count_bytes(stdin);
		print_counts(&totals, &opts);
		putchar('\n');
		return EXIT_SUCCESS;
	}

	struct counts cnts;
	for (int i = 0; i < argc; ++i) {
		if ((fp = fopen(argv[i], "r")) == NULL) {
			perror(argv[i]);
			return EXIT_FAILURE;
		}

		cnts = (opts.mflag)
		    ? count_chars(fp)
		    : count_bytes(fp);
		print_counts(&cnts, &opts);
		printf("%s\n", argv[i]);

		totals.lines += cnts.lines;
		totals.words += cnts.words;
		totals.chars += cnts.chars;

		fclose(fp);
	}

	if (argc > 1) {
		print_counts(&totals, &opts);
		printf("total\n");
	}

	return EXIT_SUCCESS;
}
