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

static bool verify_number(char *num);

static bool
verify_number(char *num)
{
	/* Plus sign allowed */
	if (num[0] == '+')
		num++;

	for (size_t i = 0; num[i] != '\0'; i++) {
		if (!isdigit(num[i]))
			return false;
	}
	return true;
}

int
sleep_main(int argc, char *argv[])
{
	if (argc < 2) {
		fputs("missing operand\n", stderr);
		return EXIT_FAILURE;
	}

	if (!verify_number(argv[1])) {
		fprintf(stderr, "sleep: invalid number '%s'\n", argv[1]);
		return EXIT_FAILURE;
	}

	sleep(atoi(argv[1]));

	return EXIT_SUCCESS;
}
