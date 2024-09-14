/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * January 1, 1 - September 2, 1752: Julian calendar
 * September 14, 1752 - December 31, 9999: Gregorian calendar
 */

static bool verify_number(char *num);
static char *get_month_name(int month);

static bool
verify_number(char *num)
{
	while (*num) {
		if (!isdigit(*num))
			return false;
		++num;
	}

	return true;
}

static char *
get_month_name(int month)
{
	static const char *months[] = {
	    "January", "February", "March", "April", "May", "June", "July",
	    "August", "September", "October", "November", "December",
	};

	if (month < 1 || month > 12)
		return "invalid month";

	return (char *)months[month - 1];
}

int
cal_main(int argc, char *argv[])
{
	int year, month;

	--argc;
	++argv;

	/* Verify numbers */
	for (int i = 0; i < argc; ++i) {
		if (!verify_number(argv[i])) {
			fprintf(stderr, "invalid number: %s\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	switch (argc) {
	case 0:
		/* current month year */
		{
			time_t t;
			struct tm *tm;

			t = time(NULL);
			tm = localtime(&t);
			year = tm->tm_year + 1900;
			month = tm->tm_mon + 1;
		}
		printf("    %s %d\n", get_month_name(month), year);
		break;
	case 1:
		/* year */
		year = atoi(argv[0]);
		month = 0;
		printf("                             %d\n", year);
		break;
	case 2:
		/* month year */
		year = atoi(argv[1]);
		month = atoi(argv[0]);
		printf("    %s %d\n", get_month_name(month), year);
		break;
	}

	return EXIT_SUCCESS;
}
