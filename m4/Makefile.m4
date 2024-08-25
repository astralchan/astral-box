dnl Copyright (C) astral
dnl See COPYING for details.
include(`m4/utils.m4')
include(`Makefile.in')

define(`print_util', `src/@1')

all: src/astral-box \
	print_util(utils)
