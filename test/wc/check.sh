#!/bin/sh

. test/common.sh

echo "Generating test files for $cmdname..."

echo "Hello, world!" > file1.txt
touch file2.txt
echo "こんにちは、世界！" > file3.txt

echo "Checking $cmdname -l option..."
check_value "1" "$($cmd -l file1.txt | awk '{print $1}')"
check_value "0" "$($cmd -l file2.txt | awk '{print $1}')"

echo "Checking $cmdname -w option..."
check_value "2" "$($cmd -w file1.txt | awk '{print $1}')"
check_value "0" "$($cmd -w file2.txt | awk '{print $1}')"
check_value "1" "$($cmd -w file3.txt | awk '{print $1}')"

echo "Checking $cmdname -c option..."
check_value "14" "$($cmd -c file1.txt | awk '{print $1}')"
check_value "0" "$($cmd -c file2.txt | awk '{print $1}')"
check_value "28" "$($cmd -c file3.txt | awk '{print $1}')"

echo "Checking $cmdname -m option..."
check_value "14" "$($cmd -m file1.txt | awk '{print $1}')"
check_value "0" "$($cmd -m file2.txt | awk '{print $1}')"
check_value "10" "$($cmd -m file3.txt | awk '{print $1}')"

echo "Checking $cmdname pipes..."
check_value "28" "$(cat file3.txt | $cmd -c | awk '{print $1}')"
check_value "10" "$(cat file3.txt | $cmd -m | awk '{print $1}')"
check_value "1 2 14" "$(echo "Hello, world!" | $cmd | awk '{print $1" "$2" "$3}')"

echo "Checking $cmdname total..."
check_value "2 3 42" "$($cmd file1.txt file2.txt file3.txt | tail -n1 | awk '{print $1" "$2" "$3}')"
