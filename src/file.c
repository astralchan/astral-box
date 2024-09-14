/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <sys/stat.h>

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * POSIX
 *
 * Nonexistent                                   cannot open
 * Block special                                 block special
 * Character special                             character special
 * Directory                                     directory
 * FIFO                                          fifo
 * Socket                                        socket
 * Symbolic link                                 symbolic link to
 * Regular file                                  regular file
 * Empty regular file                            empty
 * Regular file that cannot be read              cannot open
 *
 * Executable binary                             executable
 * ar archive library                            archive
 * Extended cpio format                          cpio archive
 * Extended tar format                           tar archive
 *
 * Shell script                                  commands text
 * C-language source                             c program text
 * FORTRAN source                                fortran program text
 *
 * Regular file whose type cannot be determined  data
 */

struct options {
	bool d;
	bool h;
	bool i;
	bool M;
	char *Marg;
	bool m;
	char *marg;
};

static void print_info(char *name, struct options *opts);

static void
print_info(char *name, struct options *opts)
{
	struct stat st;
	if (lstat(name, &st) == -1) {
		printf("cannot open");
		return;
	}

	if (access(name, F_OK) == -1) {
		printf("cannot open");
		return;
	}

	if (st.st_size == 0) {
		printf("empty");
		return;
	}

	if (S_ISDIR(st.st_mode)) {
		printf("directory");
		return;
	}

	if (S_ISREG(st.st_mode) && st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
		printf("executable");
		return;
	}

	if (S_ISLNK(st.st_mode)) {
		printf("symbolic link");
		char buf[PATH_MAX];
		ssize_t len = readlink(name, buf, sizeof(buf));
		if (len == -1) {
			fprintf(stderr, "readlink: %s: %s\n", name, strerror(errno));
			return;
		}
		buf[len] = '\0';

		struct stat rst;
		if (lstat(buf, &rst) == -1) {
			printf(" to %s: cannot open", buf);
			return;
		}

		if (opts->h || rst.st_size == 0) {
			printf(" to %s", buf);
			return;
		}

		return;
	}

	if (S_ISBLK(st.st_mode)) {
		printf("block special");
		return;
	}

	if (S_ISCHR(st.st_mode)) {
		printf("character special");
		return;
	}

	if (S_ISFIFO(st.st_mode)) {
		printf("fifo");
		return;
	}

	if (S_ISSOCK(st.st_mode)) {
		printf("socket");
		return;
	}

	if (S_ISREG(st.st_mode)) {
		printf("regular file");
		return;
	}
}

int
file_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct options opts = {
		.d = false,
		.h = false,
		.i = false,
		.M = false,
		.Marg = NULL,
		.m = false,
		.marg = NULL,
	};

	for (int opt; (opt = getopt(argc, argv, ":dhM:m:")) != -1;) {
		switch (opt) {
		case 'd':
			opts.d = true;
			break;
		case 'h':
			opts.h = true;
			break;
		case 'M':
			opts.M = true;
			opts.Marg = optarg;
			break;
		case 'm':
			opts.m = true;
			opts.marg = optarg;
			break;
		case ':':
			fprintf(stderr, "option -%c requires an argument\n", optopt);
			return EXIT_FAILURE;
		case '?':
			fprintf(stderr, "unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		fprintf(stderr, "missing operand\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < argc; ++i) {
		printf("%s: ", argv[i]);
		print_info(argv[i], &opts);
		putchar('\n');
	}

	return ret;
}
