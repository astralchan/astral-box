#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

echo "checking false return"
if ! $command; then
	echo "check passed"
else
	echo "check failed"
	exit 1
fi

exit 0
