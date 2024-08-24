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
	for (int i = 1; i < argc; ++i) {
		if (i > 1)
			putchar(' ');
		fputs(argv[i], stdout);
	}
	putchar('\n');
	return EXIT_SUCCESS;
}
