#!/bin/sh

. test/common.sh

echo "Generating $cmdname test files..."

echo "foo" > file1.txt
touch file2.txt
echo "bar" > file3.txt

echo "Checking $cmdname files..."
check_value "$(printf "foo\nbar")" "$($cmd file1.txt file2.txt file3.txt)"
