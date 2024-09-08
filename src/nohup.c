/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <sys/stat.h>

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
nohup_main(int argc, char *argv[])
{
	signal(SIGHUP, SIG_IGN);
	FILE *fp;
	char path[PATH_MAX];

	if (isatty(STDOUT_FILENO)) {
		/* S_IWUSR | S_IRUSR */
		sprintf(path, "nohup.out");
		fp = fopen(path, "a");
		if (fp == NULL) {
			fprintf(stderr, "trying $HOME/nohup.out\n");
			if (getenv("HOME") == NULL) {
				fprintf(stderr, "HOME not set\n");
				return EXIT_FAILURE;
			}
			/* concatenate HOME and nohup.out */
			sprintf(path, "%s/nohup.out", getenv("HOME"));

			fp = fopen(path, "a");
			if (fp == NULL) {
				fprintf(stderr, "cannot open %s\n", path);
				return EXIT_FAILURE;
			}
		}
	}

	--argc;
	++argv;

	/* Get size of argv */
	size_t size = 0;
	for (char **p = argv; *p; ++p)
		size += strlen(*p) + 1;

	/* Construct command with args */
	char cmd[size + 1];
	cmd[0] = '\0';
	for (int i = 0; i < argc; ++i) {
		strcat(cmd, argv[i]);
		if (i < argc - 1)
			strcat(cmd, " ");
	}
	cmd[size] = '\0';

	if (isatty(STDOUT_FILENO)) {
		dup2(fileno(fp), STDOUT_FILENO);
		dup2(fileno(fp), STDERR_FILENO);
	}

	FILE *pp = popen(cmd, "w");
	if (pp == NULL) {
		fprintf(stderr, "cannot open pipe\n");
		return EXIT_FAILURE;
	}

	pclose(pp);

	if (isatty(STDOUT_FILENO)) {
		fclose(fp);
		chmod(path, S_IRUSR | S_IWUSR);
	}

	return EXIT_SUCCESS;
}
