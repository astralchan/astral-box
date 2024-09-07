/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>

/*
 * From POSIX
 * SYNOPSIS
       who [-mTu] [-abdHlprt] [file]
       who [-mu] -s [-bHlprt] [file]
       who -q [file]
       who am i
       who am I
 */

struct options {
	bool m;
	bool T;
	bool u;
	bool a;
	bool b;
	bool d;
	bool H;
	bool l;
	bool p;
	bool r;
	bool t;
	bool s;
	bool q;
}; 

struct utmp_entry {
};

int
who_main(int argc, char *argv[])
{
	struct options opts = {
		.m = false,
		.T = false,
		.u = false,
		.a = false,
		.b = false,
		.d = false,
		.H = false,
		.l = false,
		.p = false,
		.r = false,
		.t = false,
		.s = false,
		.q = false,
	};

	/* Check for "am i" or "am I" */
	if (argc > 2)
		if (strcmp(argv[1], "am") == 0 &&
		    (strcmp(argv[2], "i") == 0 ||
		    strcmp(argv[2], "I") == 0)) {
			char *login = getlogin();
			if (login == NULL) {
				struct passwd *pw = getpwuid(getuid());
				if (pw == NULL) {
					perror("getpwuid");
					return EXIT_FAILURE;
				}
				login = pw->pw_name;
			}
			puts(login);
			return EXIT_SUCCESS;
		}

	for (int opt; (opt = getopt(argc, argv, ":mTuabdHlprtsq")) != -1;) {
		switch (opt) {
		case 'm':
			opts.m = true;
			break;
		case 'T':
			opts.T = true;
			break;
		case 'u':
			opts.u = true;
			break;
		case 'a':
			opts.a = true;
			break;
		case 'b':
			opts.b = true;
			break;
		case 'd':
			opts.d = true;
			break;
		case 'H':
			opts.H = true;
			break;
		case 'l':
			opts.l = true;
			break;
		case 'p':
			opts.p = true;
			break;
		case 'r':
			opts.r = true;
			break;
		case 't':
			opts.t = true;
			break;
		case 's':
			opts.s = true;
			break;
		case 'q':
			opts.q = true;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	/* all */
	if (opts.a) {
		opts.b =
		    opts.d =
		    opts.l =
		    opts.p =
		    opts.r =
		    opts.t =
		    opts.T =
		    opts.u =
		    true;
	}

	/* quick - set all other options to false */
	if (opts.q) {
		opts.m =
		    opts.T =
		    opts.u =
		    opts.a =
		    opts.b =
		    opts.d =
		    opts.H =
		    opts.l =
		    opts.p =
		    opts.r =
		    opts.t =
		    opts.s =
		    false;
	}

	/* TODO: implement */
	setutxent();
	for (struct utmpx *p = getutxent(); p != NULL; p = getutxent());
	endutxent();

	return EXIT_SUCCESS;
}
