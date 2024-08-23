#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

check_value "$(pwd)" "$($command)"

exit 0
