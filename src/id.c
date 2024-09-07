/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <grp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

struct options {
	bool G;
	bool g;
	bool u;
	bool n;
	bool r;
};

struct list {
	uid_t uid;
	uid_t euid;
	gid_t gid;
	gid_t egid;
	gid_t *grps;
};

static void print_list(struct list *lst, struct options *opts);

static void
print_list(struct list *lst, struct options *opts)
{
	if (opts->G + opts->g + opts->u == 0) {
		printf(
		    "uid=%u(%s) gid=%u(%s) groups=",
		    lst->uid,
		    getpwuid(lst->uid)->pw_name,
		    lst->gid,
		    getgrgid(lst->gid)->gr_name
		);

		/* Iterate through groups */
		for (int i = 0; lst->grps[i] != 0; ++i) {
			printf(
			    "%u(%s)%s",
			    lst->grps[i],
			    getgrgid(lst->grps[i])->gr_name,
			    (lst->grps[i + 1] != 0) ? "," : ""
			);
		}

		putchar('\n');
		return;
	}

	if (opts->G) {
		for (int i = 0; lst->grps[i] != 0; ++i) {
			if (opts->n)
				printf("%s", getgrgid(lst->grps[i])->gr_name);
			else
				printf("%u", lst->grps[i]);

			putchar((lst->grps[i + 1] != 0) ? ' ' : '\n');
		}
		return;
	}

	if (opts->g) {
		if (opts->n)
			printf("%s", getgrgid((opts->r) ? lst->gid : lst->egid)->gr_name);
		else
			printf("%u", (opts->r) ? lst->gid : lst->egid);
		putchar('\n');
		return;
	}

	if (opts->u) {
		if (opts->n)
			printf("%s", getpwuid((opts->r) ? lst->uid : lst->euid)->pw_name);
		else
			printf("%u", (opts->r) ? lst->uid : lst->euid);
		putchar('\n');
		return;
	}
}

int
id_main(int argc, char *argv[])
{
	struct options opts = {
		.G = false,
		.g = false,
		.u = false,
		.n = false,
		.r = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":Ggunr")) != -1;) {
		switch (opt) {
		case 'G':
			opts.G = true;
			break;
		case 'g':
			opts.g = true;
			break;
		case 'u':
			opts.u = true;
			break;
		case 'n':
			opts.n = true;
			break;
		case 'r':
			opts.r = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	if (opts.G + opts.g + opts.u > 1) {
		fprintf(stderr, "cannot specify more than one of -G, -g, and -u\n");
		return EXIT_FAILURE;
	}

	if ((opts.G + opts.g + opts.u == 0) && (opts.n || opts.r)) {
		fprintf(stderr, "cannot specify -n or -r without -G, -g, or -u\n");
		return EXIT_FAILURE;
	}

	struct passwd *pw = (argc > 0) ? getpwnam(argv[0]) : getpwuid(getuid());
	if (pw == NULL) {
		perror("passwd");
		return EXIT_FAILURE;
	}

	struct list lst = {
		.uid = pw->pw_uid,
		.euid = geteuid(),
		.gid = pw->pw_gid,
		.egid = getegid(),
		.grps = NULL,
	};

	int gidsetsize = getgroups(0, NULL);
	if (gidsetsize == -1) {
		perror("getgroups");
		return EXIT_FAILURE;
	}

	lst.grps = malloc((gidsetsize+1) * sizeof(gid_t));
	if (lst.grps == NULL) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	if (getgroups(gidsetsize, lst.grps) == -1) {
		perror("getgroups");
		free(lst.grps);
		return EXIT_FAILURE;
	}

	lst.grps[gidsetsize] = 0;

	print_list(&lst, &opts);
	free(lst.grps);

	return EXIT_SUCCESS;
}
