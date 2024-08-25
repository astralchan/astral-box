dnl Copyright (C) astral
dnl See COPYING for details.

include(`m4/utils.m4')

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int
main(int argc, char *argv[])
{
	char *cmd = strrchr(argv[0], '/');
	cmd = cmd ? cmd + 1 : argv[0];

	return EXIT_SUCCESS;
}
