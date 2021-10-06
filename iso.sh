#!/bin/sh
set -e 
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/deltos.kernel isodir/boot/deltos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "deltos" {
	multiboot /boot/deltos.kernel
}
EOF
grub-mkrescue -o deltos.iso isodir
