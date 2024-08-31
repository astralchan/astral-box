/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EPSILON 1e-9

struct options {
	bool f;
	char *farg;
	bool s;
	char *sarg;
	bool w;
};

struct range {
	double start;
	double increment;
	double end;
};

static bool verify_number(const char *str);
static int number_digits(double num);
static int max_digits(struct range *rng);

static bool
verify_number(const char *str)
{
	double tmp;
	return sscanf(str, "%lf", &tmp) == 1;
}

static int
number_digits(double num)
{
	if (fabs(num) < EPSILON)
		return 1;

	int ret = 0;
	/* Account for negative sign */
	if (num < -EPSILON)
		++ret;

	/* Count digits */
	ret += floor(log10(fabs(num))) + 1;

	return ret;
}

static int
max_digits(struct range *rng)
{
	int ret = 0;
	/* Increasing loop */
	if (rng->increment > EPSILON) {
		for (double i = rng->start; i < rng->end + EPSILON; i += rng->increment) {
			int digits = number_digits(i);
			if (digits > ret)
				ret = digits;
		}
	}
	/* Decreasing loop */
	if (rng->increment < -EPSILON) {
		for (double i = rng->start; i > rng->end - EPSILON; i += rng->increment) {
			int digits = number_digits(i);
			if (digits > ret)
				ret = digits;
		}
	}

	return ret;
}

int
seq_main(int argc, char *argv[])
{
	struct options opts = {
		.f = false,
		.farg = "%g",
		.s = false,
		.sarg = "\n",
		.w = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":f:s:w")) != -1;) {
		switch (opt) {
		case 'f':
			opts.f = true;
			opts.farg = optarg;
			break;
		case 's':
			opts.s = true;
			opts.sarg = optarg;
			break;
		case 'w':
			opts.w = true;
			break;
		case ':':
			fprintf(stderr, "option: -%c requires an argument\n", optopt);
			return EXIT_FAILURE;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	if (opts.f && opts.w) {
		fprintf(stderr, "cannot use -f and -w together\n");
		return EXIT_FAILURE;
	}

	if (argc < 1) {
		fprintf(stderr, "missing range\n");
		return EXIT_FAILURE;
	}

	/* Veryify args are doubles */
	for (int i = 0; i < argc; ++i) {
		if (!verify_number(argv[i])) {
			fprintf(stderr, "invalid number: %s\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	struct range rng = {
		.start = 1.0,
		.increment = 1.0,
		.end = 1.0,
	};

	switch (argc) {
	/* end */
	case 1:
		rng.end = strtod(argv[0], NULL);
		break;
	/* start end */
	case 2:
		rng.start = strtod(argv[0], NULL);
		rng.end = strtod(argv[1], NULL);
		break;
	/* start increment end */
	case 3:
		rng.start = strtod(argv[0], NULL);
		rng.increment = strtod(argv[1], NULL);
		rng.end = strtod(argv[2], NULL);
		break;
	default:
		fprintf(stderr, "too many arguments\n");
		return EXIT_FAILURE;
	}

	int digits = 0;
	/* digits not needed if w isn't specified */
	if (opts.w)
		digits = max_digits(&rng);

	/* "Forever" loop */
	if (fabs(rng.increment) < EPSILON) {
		fprintf(stderr, "increment too small\n");
		return EXIT_FAILURE;
	}

	/* Increasing loop */
	if (rng.increment > EPSILON) {
		for (double i = rng.start; i < rng.end + EPSILON; i += rng.increment) {
			if (opts.w)
				printf("%0*.*g", digits, digits, i);
			else
				printf(opts.farg, i);
			if (i + rng.increment <= rng.end)
				fputs(opts.sarg, stdout);
		}
	}

	/* Decreasing loop */
	if (rng.increment < -EPSILON) {
		for (double i = rng.start; i > rng.end - EPSILON; i += rng.increment) {
			if (opts.w)
				printf("%0*.*g", digits, digits, i);
			else
				printf(opts.farg, i);
			if (i + rng.increment >= rng.end)
				fputs(opts.sarg, stdout);
		}
	}

	putchar('\n');

	return EXIT_SUCCESS;
}
