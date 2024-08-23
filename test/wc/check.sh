#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

# Make test files
echo "Hello, world!" > file1.txt
touch file2.txt
echo "こんにちは、世界！" > file3.txt

echo "Checking wc -c"
check_value "14" "$($command -c file1.txt | awk '{print $1}')"
check_value "0" "$($command -c file2.txt | awk '{print $1}')"
check_value "28" "$($command -c file3.txt | awk '{print $1}')"

echo "Checking wc -l"
check_value "1" "$($command -l file1.txt | awk '{print $1}')"
check_value "0" "$($command -l file2.txt | awk '{print $1}')"
check_value "1" "$($command -l file3.txt | awk '{print $1}')"

echo "Checking wc -w"
check_value "2" "$($command -w file1.txt | awk '{print $1}')"
check_value "0" "$($command -w file2.txt | awk '{print $1}')"
check_value "1" "$($command -w file3.txt | awk '{print $1}')"

echo "Checking wc -m"
check_value "14" "$($command -m file1.txt | awk '{print $1}')"
check_value "0" "$($command -m file2.txt | awk '{print $1}')"
check_value "10" "$($command -m file3.txt | awk '{print $1}')"

echo "Checking wc multiple files"
check_value "2 3 42" "$($command file1.txt file2.txt file3.txt | tail -n 1 | awk '{print $1" "$2" "$3}')"
check_value "2 3 24" "$($command -lwm file1.txt file2.txt file3.txt | tail -n 1 | awk '{print $1" "$2" "$3}')"

echo "Checking pipes for wc"
check_value "1 2 14" "$(cat file1.txt | $command | awk '{print $1" "$2" "$3}')"
check_value "0 0 0" "$(cat file2.txt | $command | awk '{print $1" "$2" "$3}')"
check_value "1 1 28" "$(cat file3.txt | $command | awk '{print $1" "$2" "$3}')"

clean_up
exit 0
