/*
 * Copyright (C) astral
 * See COPYING for details.
 */

/*
 * TODO
 * - recursion (R)
 * - sorting (S, f, t)
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* Linux does not implement stropts.h */
#define WIDTH 80

#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[30m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

struct options {
	/* General */
	bool i;
	bool k;
	bool q;
	bool r;
	bool s;
	/* Long */
	bool g;
	bool l;
	bool n;
	bool o;
	/* Hidden */
	bool A;
	bool a;
	/* Column */
	bool C;
	bool m;
	bool x;
	bool _1;
	/* Append chars */
	bool F;
	bool p;
	/* Follow symlinks */
	bool H;
	bool L;
	/* Recursion / file */
	bool R;
	bool d;
	/* Sorting */
	bool S;
	bool f;
	bool t;
	/* Time */
	bool c;
	bool u;
};

static void print_permissions(struct stat *st);
static void print_time(struct stat *st, struct options *opts);
static void append_char(char *path, struct stat *st, struct options *opts);
static void print_entry(char *entry, struct options *opts, int *status);

static void
print_permissions(struct stat *st)
{
	putchar((S_ISDIR(st->st_mode)) ? 'd' :
	    (S_ISLNK(st->st_mode)) ? 'l' :
	    (S_ISREG(st->st_mode)) ? '-' :
	    (S_ISBLK(st->st_mode)) ? 'b' :
	    (S_ISCHR(st->st_mode)) ? 'c' :
	    (S_ISFIFO(st->st_mode)) ? 'p' :
	    (S_ISSOCK(st->st_mode)) ? 's' : '?');

	putchar((st->st_mode & S_IRUSR) ? 'r' : '-');
	putchar((st->st_mode & S_IWUSR) ? 'w' : '-');
	putchar((st->st_mode & S_IXUSR) ? 'x' :
	        (st->st_mode & S_ISUID) ? 'S' : '-');

	putchar((st->st_mode & S_IRGRP) ? 'r' : '-');
	putchar((st->st_mode & S_IWGRP) ? 'w' : '-');
	putchar((st->st_mode & S_IXGRP) ? 'x' :
	        (st->st_mode & S_ISGID) ? 'S' : '-');

	putchar((st->st_mode & S_IROTH) ? 'r' : '-');
	putchar((st->st_mode & S_IWOTH) ? 'w' : '-');
	putchar((st->st_mode & S_IXOTH) ? 'x' : '-');
}

static void
print_time(struct stat *st, struct options *opts)
{
	struct tm *tm;

	tm = localtime(
	    (opts->c) ? &st->st_ctime :
	    (opts->u) ? &st->st_atime :
	    &st->st_mtime
	);

	char time[128];
	strftime(time, sizeof(time), "%b %d %H:%M", tm);
	fputs(time, stdout);
}

static void
append_char(char *path, struct stat *st, struct options *opts)
{
	if (opts->p && S_ISDIR(st->st_mode))
		strcat(path, "/");

	if (opts->F) {
		if (S_ISDIR(st->st_mode))
			strcat(path, "/");
		if (S_ISLNK(st->st_mode))
			strcat(path, "@");
		if (S_ISREG(st->st_mode) &&
		    st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
			strcat(path, "*");
		if (S_ISFIFO(st->st_mode))
			strcat(path, "|");
		if (S_ISSOCK(st->st_mode))
			strcat(path, "=");
		if (S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode))
			strcat(path, "#");
	}
}

static void
print_entry(char *entry, struct options *opts, int *status)
{
	struct stat st;
	if (opts->L) {
		if (stat(entry, &st) == -1) {
			fprintf(stderr, "stat: %s: %s\n", entry, strerror(errno));
			*status = EXIT_FAILURE;
			return;
		}
	} else {
		if (lstat(entry, &st) == -1) {
			fprintf(stderr, "lstat: %s: %s\n", entry, strerror(errno));
			*status = EXIT_FAILURE;
			return;
		}
	}

	DIR *dir;
	struct dirent **namelist;
	char path[PATH_MAX];
	int entries;
	size_t blockTotal = 0;
	if (S_ISDIR(st.st_mode) && !opts->d) {
		dir = opendir(entry);
		if (dir == NULL) {
			fprintf(stderr, "opendir: %s: %s\n", entry, strerror(errno));
			*status = EXIT_FAILURE;
			return;
		}

		entries = scandir(entry, &namelist, NULL, alphasort);
	} else { /* File */
		entries = 1;
		if (opts->i)
			printf("%lu ", st.st_ino);

		if (opts->s)
			printf("%lu ", st.st_blocks);

		append_char(entry, &st, opts);

		/* Long format */
		
		if (opts->g || opts->l || opts->n || opts->o) {
			/* Permissions */

			/* File type */
			print_permissions(&st);

			putchar(' ');

			/* Number of links */
			printf("%4lu ", st.st_nlink);

			if (!opts->g) {
				if (opts->n)
					printf("%u ", st.st_uid);
				else
					printf("%s ", getpwuid(st.st_uid)->pw_name);
			}

			if (!opts->o) {
				if (opts->n)
					printf("%u ", st.st_gid);
				else
					printf("%s ", getgrgid(st.st_gid)->gr_name);
			}

			/* Size or device info */
			if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode))
				printf("%*lu ", 8, st.st_dev);
			else
				printf("%*lu ", 8, st.st_size);

			/* Print last access time */
			print_time(&st, opts);
			putchar(' ');

			/* Link arrow */
			if (!opts->L && S_ISLNK(st.st_mode)) {
				if (readlink(entry, path, sizeof(path)) == -1) {
					fprintf(stderr, "readlink: %s: %s\n", entry, strerror(errno));
					*status = EXIT_FAILURE;
					return;
				}

				printf("%s -> %s", entry, path);
			}

			fputs(entry, stdout);
		}

		putchar('\n');
		return;
	}

	if (entries == -1) {
		fprintf(stderr, "scandir: %s: %s\n", entry, strerror(errno));
		*status = EXIT_FAILURE;
		return;
	}

	/* Get largest entry name and total block size */
	int colWidth = 0;
	for (int j = 0; j < entries; ++j) {
		int len = (int)strlen(namelist[j]->d_name);

		blockTotal += (opts->k) ? st.st_blocks * 2 : st.st_blocks;

		if (opts->s)
			len += (opts->k) ?
			    (int)log10(st.st_blocks * 2) + 2 :
			    (int)log10(st.st_blocks) + 2;

		if (opts->i)
			len += (int)log10(st.st_ino) + 2;

		if (opts->p && S_ISDIR(st.st_mode))
			++len;

		if (opts->F) {
			if (S_ISDIR(st.st_mode))
				++len;
			if (S_ISLNK(st.st_mode))
				++len;
			if (S_ISREG(st.st_mode) &&
			    st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
				++len;
			if (S_ISFIFO(st.st_mode))
				++len;
			if (S_ISSOCK(st.st_mode))
				++len;
			if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode))
				++len;
		}

		if (len > colWidth)
			colWidth = len;
	}

	colWidth += 2;

	/* Print total block size */
	if (opts->g || opts->l || opts->n || opts->o) {
		printf("total %lu\n", blockTotal);
	}

	char name[PATH_MAX], nameTmp[PATH_MAX], colorName[PATH_MAX];
	for (int i = 0; i < entries; ++i) {
		sprintf(path, "%s/%s", entry, namelist[i]->d_name);

		if (namelist[i]->d_name[0] == '.' && !opts->a && !opts->A)
			continue;

		if (opts->A &&
		    (strcmp(namelist[i]->d_name, ".") == 0 ||
		    strcmp(namelist[i]->d_name, "..") == 0))
			continue;

		if (opts->L) {
			if (stat(path, &st) == -1) {
				fprintf(stderr, "stat: %s: %s\n", entry, strerror(errno));
				*status = EXIT_FAILURE;
				return;
			}
		} else {
			if (lstat(path, &st) == -1) {
				fprintf(stderr, "lstat: %s: %s\n", entry, strerror(errno));
				*status = EXIT_FAILURE;
				return;
			}
		}

		/*
		 * Construct entry name
		 */

		strcpy(name, namelist[i]->d_name);
		strcpy(nameTmp, name);

		/* Append chars */
		append_char(name, &st, opts);

		strcpy(nameTmp, name);

		if (opts->s) {
			sprintf(name, "%lu %s",
			    (opts->k) ? st.st_blocks * 2 : st.st_blocks,
			    nameTmp);
			strcpy(nameTmp, name);
		}

		if (opts->i) {
			sprintf(name, "%lu %s", st.st_ino, nameTmp);
			strcpy(nameTmp, name);
		}

		/* Print entry */

		if (opts->C || opts->x) {
			/* Column format */

			printf("%-*s", colWidth, name);
		} else if (opts->g || opts->l || opts->n || opts->o) {
			/* Long format */

			/* Permissions */

			/* File type */
			print_permissions(&st);

			putchar(' ');

			/* Number of links */
			printf("%4lu ", st.st_nlink);

			if (!opts->g) {
				if (opts->n)
					printf("%u ", st.st_uid);
				else
					printf("%s ", getpwuid(st.st_uid)->pw_name);
			}

			if (!opts->o) {
				if (opts->n)
					printf("%u ", st.st_gid);
				else
					printf("%s ", getgrgid(st.st_gid)->gr_name);
			}

			/* Size or device info */
			if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode))
				printf("%*lu ", 8, st.st_dev);
			else
				printf("%*lu ", 8, st.st_size);

			/* Print last access time */
			print_time(&st, opts);
			putchar(' ');

			/* Link arrow */
			if (!opts->L && S_ISLNK(st.st_mode)) {
				if (readlink(path, nameTmp, sizeof(nameTmp)) == -1) {
					fprintf(stderr, "readlink: %s: %s\n", path, strerror(errno));
					*status = EXIT_FAILURE;
					return;
				}

				sprintf(name, "%s -> %s", name, nameTmp);
			}

			fputs(name, stdout);
		} else {
			/* One entry per line */

			fputs(name, stdout);
		}

		if (opts->C || opts->x) {
			/* Print new row */
			if (i % (WIDTH / colWidth) == 0 && i != 0)
				putchar('\n');
		} else if (i < entries - 1) {
			fputs(
			    (opts->_1) ? "\n" :
			    (opts->m) ? ", " :
			    "\n",
			    stdout
			);
		}
	}

	free(namelist);

	putchar('\n');
}

int
ls_main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;

	struct options opts = {
		.i = false,
		.k = false,
		.q = false,
		.r = false,
		.s = false,
		.g = false,
		.l = false,
		.n = false,
		.o = false,
		.A = false,
		.a = false,
		.C = false,
		.m = false,
		.x = false,
		._1 = false,
		.F = false,
		.p = false,
		.H = false,
		.L = false,
		.R = false,
		.d = false,
		.S = false,
		.f = false,
		.t = false,
		.c = false,
		.u = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":ikqrsglnoAaCmx1FpHLRdSftcu")) != -1;) {
		switch (opt) {
		case 'i':
			opts.i = true;
			break;
		case 'k':
			opts.k = true;
			break;
		case 'q':
			opts.q = true;
			break;
		case 'r':
			opts.r = true;
			break;
		case 's':
			opts.s = true;
			break;
		case 'g':
			opts.g = true;
			opts.C = false;
			opts.m = false;
			opts.x = false;
			break;
		case 'l':
			opts.l = true;
			opts.C = false;
			opts.m = false;
			opts.x = false;
			break;
		case 'n':
			opts.n = true;
			opts.C = false;
			opts.m = false;
			opts.x = false;
			break;
		case 'o':
			opts.o = true;
			opts.C = false;
			opts.m = false;
			opts.x = false;
			break;
		case 'A':
			opts.A = true;
			opts.a = false;
			break;
		case 'a':
			opts.a = true;
			opts.A = false;
			break;
		case 'C':
			opts.C = true;
			opts.l = false;
			opts.g = false;
			opts.n = false;
			opts.o = false;
			opts.m = false;
			opts.x = false;
			opts._1 = false;
			break;
		case 'm':
			opts.m = true;
			opts.C = false;
			opts.l = false;
			opts.g = false;
			opts.n = false;
			opts.o = false;
			opts.x = false;
			opts._1 = false;
			break;
		case 'x':
			opts.x = true;
			opts.C = false;
			opts.l = false;
			opts.g = false;
			opts.n = false;
			opts.o = false;
			opts.m = false;
			opts._1 = false;
			break;
		case '1':
			opts._1 = true;
			opts.C = false;
			opts.m = false;
			opts.x = false;
			break;
		case 'F':
			opts.F = true;
			opts.p = false;
			break;
		case 'p':
			opts.p = true;
			opts.F = false;
			break;
		case 'H':
			opts.H = true;
			opts.L = false;
			break;
		case 'L':
			opts.L = true;
			opts.H = false;
			break;
		case 'R':
			opts.R = true;
			opts.d = false;
			break;
		case 'd':
			opts.d = true;
			opts.R = false;
			break;
		case 'S':
			opts.S = true;
			opts.f = false;
			opts.t = false;
			break;
		case 'f':
			opts.f = true;
			opts.S = false;
			opts.t = false;
			break;
		case 't':
			opts.t = true;
			opts.S = false;
			opts.f = false;
			break;
		case 'c':
			opts.c = true;
			opts.u = false;
			break;
		case 'u':
			opts.u = true;
			opts.c = false;
			break;
		case '?':
			fprintf(stderr, "unknown option: -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	if (isatty(STDOUT_FILENO) &&
	    !opts.g &&
	    !opts.l &&
	    !opts.n &&
	    !opts.o &&
	    !opts.C &&
	    !opts.m &&
	    !opts.x &&
	    !opts._1)
		opts.C = true;

	if (argc < 1) {
		print_entry(".", &opts, &ret);
		return ret;
	}

	int retTmp = EXIT_SUCCESS;
	for (int i = 0; i < argc; ++i) {
		if (argc > 1)
			printf("%s:\n", argv[i]);
		print_entry(argv[i], &opts, &retTmp);
		if (retTmp == EXIT_FAILURE)
			ret = EXIT_FAILURE;
		if (argc > 1 && i < argc - 1)
			putchar('\n');
	}

	return ret;
}
