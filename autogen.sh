#! /bin/bash

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

echo "running: aclocal"
aclocal

echo "running: libtoolize --force"
libtoolize --force

echo "running: automake --add-missing"
automake --add-missing

echo "running: autoheader"
autoheader

echo "running: autoconf"
autoconf

echo "running: $srcdir/configure $*"
$srcdir/configure $*
