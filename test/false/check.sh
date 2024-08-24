#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

echo "Checking false return"
if ! $command; then
	echo "Check passed"
else
	echo "Check failed"
	exit 1
fi

exit 0
