#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

echo "Checking printf format"
check_value "$(printf "Hello, world!\n")" "$($command "%s, %s!\n" "Hello" "world")"
check_value "$(printf "1, 2, 3\n")" "$($command "%d, %d, %d\n" 1 2 3)"
check_value "42" "$($command "%ld" 42)"

exit 0
