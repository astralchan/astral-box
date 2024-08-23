#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

current_dir=$(pwd)
test_dir=$(dirname $0)

check_value() {
	if [ "$1" = "$2" ]; then
		echo "check passed"
	else
		echo "check failed"
		exit 1
	fi
}

exit 0
