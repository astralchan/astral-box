#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

echo "Checking ranges for seq"
check_value "$(printf "1\n2\n3\n")" "$($command 3)"
check_value "$(printf "3\n4\n5\n")" "$($command 3 5)"
check_value "$(printf "2\n2.5\n3\n3.5\n4\n4.5\n5\n")" "$($command 2 0.5 5)"

echo "Checking seq -w"
check_value "$(printf "001\n002\n003\n004\n")" "$($command -w 100 | head -n 4)"

echo "Checking seq -s"
check_value "1:2:3:4" "$($command -s ":" 4)"

echo "Checking seq pasring"
check_value "$(printf "7\n8\n9\n10\n")" "$($command 7 1e1)"
check_value "$(printf "11\n11.5\n12\n")" "$($command 11 0.5 +1.2e1)"

exit 0
