#!/bin/sh

# Copyright (C) astral
# See COPYING for details.

. test/common.sh

cat << _EOF
current_dir: $current_dir
test_dir: $test_dir
command: $command
pwd: `pwd`
_EOF

exit 0
