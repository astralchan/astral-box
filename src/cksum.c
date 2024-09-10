/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO
 * Learn about CRC32
 */

/*
 * CRC32 generator polynomial powers
 * 32 26 23 22 16 12 11 10 8 7 5 4 2 1 0
 */

#define BUFSIZE 4096

struct sum {
	uint32_t crc;
	uint64_t bytes;
};

static struct sum get_sum(FILE *fp);
static void print_sum(struct sum *sm, char *name);

static struct sum
get_sum(FILE *fp)
{
	struct sum ret = {
		.crc = 0,
		.bytes = 0
	};

	uint8_t buf[BUFSIZE];
	for (size_t n; (n = fread(buf, 1, BUFSIZE, fp)) > 0; ) {
		ret.bytes += n;
	}

	return ret;
}

static void
print_sum(struct sum *sm, char *name)
{
	printf("%u %lu %s\n", sm->crc, sm->bytes, name);
}

int
cksum_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct sum sm;
	if (argc < 2) {
		sm = get_sum(stdin);
		print_sum(&sm, "-");
		return EXIT_SUCCESS;
	}

	FILE *fp;
	bool useStdin = false;
	for (int i = 1; i < argc; ++i) {
		useStdin = strcmp(argv[i], "-") == 0;

		if (!useStdin) {
			fp = fopen(argv[i], "rb");
			if (fp == NULL) {
				perror(argv[i]);
				ret = EXIT_FAILURE;
				continue;
			}
		}

		if (useStdin)
			fp = stdin;

		sm = get_sum(fp);
		print_sum(&sm, argv[i]);

		if (!useStdin)
			fclose(fp);
	}

	return ret;
}
