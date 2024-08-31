/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void asa_file(FILE *fp);

static void
asa_file(FILE *fp)
{
	char *buf = NULL;
	size_t len = 0;
	ssize_t nread;

	while ((nread = getline(&buf, &len, fp)) != -1) {
		switch (buf[0]) {
		case ' ':
			fputs(buf+1, stdout);
			break;
		case '0':
			printf("\n%s", buf+1);
			break;
		}
	}

	free(buf);
}

int
asa_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	if (argc < 2) {
		asa_file(stdin);
		return ret;
	}

	bool useStdin = false;
	FILE *fp;
	for (int i = 1; i < argc; ++i) {
		useStdin = strcmp(argv[i], "-") == 0;

		if (!useStdin) {
			fp = fopen(argv[i], "r");
			if (fp == NULL) {
				perror(argv[i]);
				ret = EXIT_FAILURE;
				continue;
			}
		}

		if (useStdin)
			fp = stdin;

		asa_file(fp);

		if (!useStdin)
			fclose(fp);
	}

	return ret;
}
