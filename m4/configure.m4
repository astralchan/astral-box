dnl Copyright (C) astral
dnl See COPYING for details.
dnl
include(`configure.in')
dnl
define(`utils', `
 false
 ls
 true
 wc
 cat
 seq
 pwd
 cksum
 cal
 printf
 echo
 head
 tail
')
dnl
echo "utilities:"
cat << _EOF
utils
_EOF
dnl
cat << _EOF >> Makefile
_EOF
