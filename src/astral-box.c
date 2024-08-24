/*
 * Copyright (C) astral
 * See COPYING for details.
 */

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

	if (!strcmp(cmd, "cat"))
		return cat_main(argc, argv);
	else if (!strcmp(cmd, "false"))
		return false_main();
	else if (!strcmp(cmd, "ls"))
		return ls_main(argc, argv);
	else if (!strcmp(cmd, "pwd"))
		return pwd_main(argc, argv);
	else if (!strcmp(cmd, "seq"))
		return seq_main(argc, argv);
	else if (!strcmp(cmd, "true"))
		return true_main();
	else if (!strcmp(cmd, "wc"))
		return wc_main(argc, argv);
	else if (!strcmp(cmd, "cksum"))
		return cksum_main(argc, argv);
	else if (!strcmp(cmd, "printf"))
		return printf_main(argc, argv);
	else if (!strcmp(cmd, "echo"))
		return echo_main(argc, argv);
	else if (!strcmp(cmd, "head"))
		return head_main(argc, argv);
	else if (!strcmp(cmd, "tail"))
		return tail_main(argc, argv);
	else {
		fprintf(stderr, "astral-box: %s: command not found\n", cmd);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
