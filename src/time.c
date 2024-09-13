/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct options {
	bool p;
};

static void start_clock(struct tms *st_cpu, clock_t *st_time);
static void end_clock(struct tms *st_cpu, clock_t *st_time, struct tms *en_cpu, clock_t *en_time, struct options *opts);

static void
start_clock(struct tms *st_cpu, clock_t *st_time)
{
	*st_time = times(st_cpu);
}

static void
end_clock(struct tms *st_cpu, clock_t *st_time, struct tms *en_cpu, clock_t *en_time, struct options *opts)
{
	*en_time = times(en_cpu);

	/* Get precision */
	long clk_tck = sysconf(_SC_CLK_TCK);

	/* Print in seconds */
	fprintf(stderr,
	    (opts->p) ? "real %f\nuser %f\nsys %f\n" :
	    "real %.2fs\nuser %.2fs\nsys %.2fs\n",
	    (double)(*en_time - *st_time) / clk_tck,
	    (double)(en_cpu->tms_utime - st_cpu->tms_utime) / clk_tck,
	    (double)(en_cpu->tms_stime - st_cpu->tms_stime) / clk_tck);
}

int
time_main(int argc, char *argv[])
{
	struct options opts = {
		.p = false,
	};

	for (int opt; (opt = getopt(argc, argv, "p")) != -1;) {
		switch (opt) {
		case 'p':
			opts.p = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: %c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	clock_t st_time;
	clock_t en_time;
	struct tms st_cpu;
	struct tms en_cpu;

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

	start_clock(&st_cpu, &st_time);

	system(cmd);

	end_clock(&st_cpu, &st_time, &en_cpu, &en_time, &opts);

	return EXIT_SUCCESS;
}
