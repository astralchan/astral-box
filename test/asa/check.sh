#!/bin/sh

. test/common.sh

echo "Generating $cmdname test files..."

echo " foo" > file1.txt
echo "0foo" > file2.txt

check_value "foo" "$($cmd file1.txt)"
check_value "$(printf "\nfoo")" "$($cmd file2.txt)"
