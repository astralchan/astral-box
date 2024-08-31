#!/bin/sh

. test/common.sh

# cksum is not complete
# only octal dump is complete

echo "Generating $cmdname test files..."
echo "Hello, world!" > file1.txt
touch file2.txt

echo "Checking $cmdname octal dump..."
check_value "14" "$($cmdname file1.txt | awk '{print $2}')"
check_value "0" "$($cmdname file2.txt | awk '{print $2}')"

echo "Checking $cmdname pipes..."
check_value "6" "$(printf "foobar" | $cmdname | awk '{print $2}')"
check_value "0" "$(printf "" | $cmdname | awk '{print $2}')"
