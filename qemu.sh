#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-ro-arch.sh $HOST) -cdrom deltos.iso
