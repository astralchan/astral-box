/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EPSILON (10e-7)

struct options {
	bool fflag;
	bool sflag;
	bool wflag;
};

static bool isnum(const char *str);
static double parse_num(const char *str);

/*
 * check if string is decimal, floating, or scientific notation
 */
static bool
isnum(const char *str)
{
	/*
	 * check if string is a valid number
	 */

	bool dot = false;
	bool e = false;
	bool sign = false;

	/*
	 * first-pass:
	 *   - check for [0-9], '.', 'e', 'E', '+', '-'
	 *   - check for one '.' and one 'e' at max
	 */
	for (size_t i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i]) && str[i] != '.' && str[i] != 'e' &&
		    str[i] != 'E' && str[i] != '+' && str[i] != '-') {
			return false;
		}
		/* One dot allowed */
		if (str[i] == '.') {
			if (dot)
				return false;
			dot = true;
		}
		/* One e allowed */
		if (str[i] == 'e' || str[i] == 'E') {
			if (e)
				return false;
			e = true;
		}
	}

	return true;
}

static double
parse_num(const char *str)
{
	return strtold(str, NULL);
}

int
seq_main(int argc, char *argv[])
{
	struct options opts = {
		.fflag = false,
		.sflag = false,
		.wflag = false,
	};

	char *format, *separator;

	for (int opt; (opt = getopt(argc, argv, ":f:s:w")) != -1;) {
		switch (opt) {
		case 'f':
			opts.fflag = true;
			format = optarg;
			break;
		case 's':
			opts.sflag = true;
			separator = optarg;
			break;
		case 'w':
			opts.wflag = true;
			break;
		case ':':
			fprintf(stderr, "seq: option -%c requires an argument\n", optopt);
			return EXIT_FAILURE;
		case '?':
			fprintf(stderr, "seq: unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	if (opts.fflag && opts.wflag) {
		fprintf(stderr, "cannot use -f and -w together\n");
		return EXIT_FAILURE;
	}

	if (!opts.fflag)
		format = "%g";

	if (!opts.sflag)
		separator = "\n";

	argc -= optind;
	argv += optind;

	double lower, upper, increment;
	lower = upper = increment = 1;

	if (argc == 0) {
		fprintf(stderr, "missing operand\n");
		return EXIT_FAILURE;
	}

	if (argc > 3) {
		fprintf(stderr, "extra operand '%s'\n", argv[3]);
		return EXIT_FAILURE;
	}

	/* Validate numbers */
	for (int i = 0; i < argc; i++) {
		if (!isnum(argv[i])) {
			fprintf(stderr, "invalid number: '%s'\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	/* Based off of manpage */
	if (argc == 1) {
		upper = parse_num(argv[0]);
	} else if (argc == 2) {
		lower = parse_num(argv[0]);
		upper = parse_num(argv[1]);
	} else if (argc == 3) {
		lower = parse_num(argv[0]);
		increment = parse_num(argv[1]);
		upper = parse_num(argv[2]);
	}

	/* See how many 0s need to pad for -w */
	int ndigits = 0;
	long upper_cast = (long)upper;
	for (; upper_cast; upper_cast /= 10, ++ndigits);

	/* Return success for infinite / epsilon-"infinite" loops */

	if (upper+EPSILON < lower-EPSILON && increment > EPSILON)
		return EXIT_SUCCESS;

	if (upper-EPSILON > lower+EPSILON && increment < -EPSILON)
		return EXIT_SUCCESS;

	/* seq doesn't allow non-zero increment */
	if (fabs(increment) < EPSILON) {
		fprintf(stderr, "increment too small: '%g'\n", increment);
		return EXIT_FAILURE;
	}

	for (double i = lower, j = upper; i <= upper && j >= lower;
	    i += increment, j -= increment) {
		if (opts.wflag)
			printf("%0*.*g", ndigits, ndigits, i);
		else
			printf(format, i);

		/* Don't print seperator on last entry */
		if (i+EPSILON < upper-EPSILON || j-EPSILON > lower+EPSILON)
			printf("%s", separator);
	}
	putchar('\n');

	return EXIT_SUCCESS;
}
