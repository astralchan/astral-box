#!/bin/sh

. test/common.sh

echo "Generating test files..."

echo "Hello, world!" > file1.txt
touch file2.txt

echo "Checking $cmdname -m..."
check_value "check.sh, file1.txt, file2.txt" "$($cmd -m)"
