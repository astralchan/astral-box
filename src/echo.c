/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int
echo_main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		fputs(argv[i], stdout);
		if (i < argc - 1)
			putchar(' ');
	}
	putchar('\n');

	return EXIT_SUCCESS;
}
