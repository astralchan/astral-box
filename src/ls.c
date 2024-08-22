/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include <sys/stat.h>

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct options {
	bool aflag;
	bool Aflag;
};

/*
 * @brief: list the contents of a directory
 * @param: path
 * path to the directory
 * @param: aflag
 * if true, list all files including hidden files
 * @param: Aflag
 * if true, list all files except . and ..
 * @return
 * a string containing the contents of the directory, null-separated
 */
static void list_contents(const char *path, struct options *opts);

static void
list_contents(const char *path, struct options *opts)
{
	struct stat st;

	if (stat(path, &st) == -1) {
		perror("stat");
		return;
	}

	if (!S_ISDIR(st.st_mode)) {
		printf("%s\n", path);
		return;
	}

	DIR *dir = opendir(path);
	if (dir == NULL) {
		perror("opendir");
		return;
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && (!opts->aflag && (!opts->Aflag ||
			    strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)))
			continue;

		printf("%s\n", entry->d_name);
	}

	closedir(dir);
}

int
main(int argc, char *argv[])
{
	/* Initialize default options */
	struct options opts = {
		.aflag = false,
		.Aflag = false,
	};

	for (int opt; (opt = getopt(argc, argv, ":aA")) != -1;) {
		switch (opt) {
		case 'a':
			opts.aflag = true;
			break;
		case 'A':
			opts.Aflag = true;
			break;
		case '?':
			fprintf(stderr, "unknown option -'%c'\n", optopt);
			return EXIT_FAILURE;
		}
	}

	argc -= optind;
	argv += optind;

	/* Use current dir if none specified */
	if (argc == 0) {
		list_contents(".", &opts);
		return EXIT_SUCCESS;
	}

	for (int i = 0; i < argc; i++)
		list_contents(argv[i], &opts);

	return EXIT_SUCCESS;
}
