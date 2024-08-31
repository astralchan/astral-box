#!/bin/sh

. test/common.sh

echo "Checking $cmdname end..."
check_value "$(printf "1\n2\n3\n4")" "$($cmd 4)"
echo "Checking $cmdname start end..."
check_value "$(printf "6\n7\n8")" "$($cmd 6 8)"
echo "Checking $cmdname start increment end..."
check_value "$(printf "6\n8\n10")" "$($cmd 6 2 10)"
echo "Checking $cmdname -w option..."
check_value "$(printf "099\n100")" "$($cmd -w 99 100)"
check_value "$(printf -- "-1\n00\n01")" "$($cmd -w -- -1 1)"
echo "Checking $cmdname notation..."
check_value "$(printf "9\n10\n11\n12")" "$($cmd 9 1.2e1)"
echo "Checking $cmdname decimal increment..."
check_value "$(printf "2\n2.5\n3\n3.5\n4")" "$($cmd 2 0.5 4)"
