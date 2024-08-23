#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

current_dir=$(pwd)
test_dir=$(dirname $0)

pwd=$current_dir/src/pwd

check_value() {
	printf "expected: %s\ngot: %s\n" "$1" "$2"
	if [ "$1" = "$2" ]; then
		echo "check passed"
	else
		echo "check failed"
		exit 1
	fi
}

check_value "$current_dir" "$($pwd)"

exit 0
