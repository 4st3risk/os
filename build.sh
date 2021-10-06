#!/bin/sh
set -e
. ./header.sh

for(PROJECT in $PROJECTS; do
	(cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
