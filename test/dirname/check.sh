#!/bin/sh

. test/common.sh

echo "Generating $cmdname test files..."

touch file1

check_value "." "$($cmd file1)"
