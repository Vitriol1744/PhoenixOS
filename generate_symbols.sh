#!/bin/sh
echo 'generating ksyms.sym...'
tmp=$(mktemp)
NM="${NM:-nm}"
nm -n Kernel/PhoenixOS.elf > "$tmp"
printf "%08x\n" "$(wc -l "$tmp" | awk '{print $1}')" > ksyms.sym
cat "$tmp" >> ksyms.sym
rm -f "$tmp"
#cp -r ./initrd.tar.gz ./build/iso_root
