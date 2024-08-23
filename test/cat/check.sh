#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

# Make test files
echo "Hello, world!" > file1.txt
touch file2.txt
printf "Goodbye,\nworld!\n" > file3.txt

echo "Checking cat file"
check_value "Hello, world!" "$($command file1.txt)"
check_value "" "$($command file2.txt)"
check_value "$(printf "Goodbye,\nworld!\n")" "$($command file3.txt)"

echo "Checking cat pipes"
check_value "Hello, world!" "$(echo "Hello, world!" | $command)"
check_value "" "$(echo "" | $command)"

clean_up

exit 0
