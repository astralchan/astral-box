/*
 * Copyright (C) astral
 * See COPYING for details.
 */

#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* ISO/IEC 8802-3:1996 */

/*
 * Generating polynomial:
 * G(x)=x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 */
#define CKSUM_POLY 0x04C11DB7

/*
 * TODO
 * - Implement cksum
 * - Learn about generator polynomials
 */

int
cksum_main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("stdin: cksum: not implemented\n");
		return EXIT_SUCCESS;
	}

	for (int i = 1; i < argc; ++i)
		printf("%s: cksum: not implemented\n", argv[i]);

	return EXIT_SUCCESS;
}
