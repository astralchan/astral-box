#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

echo "Checking echo string"
check_value "Hello, world!" "$($command "Hello, world!")"

echo "Checking empty string"
check_value "$(printf '\n')" "$($command "")"

exit 0
