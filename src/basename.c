/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

int
basename_main(int argc, char *argv[])
{
	printf("%s\n", basename(argv[1]));
	return EXIT_SUCCESS;
}
