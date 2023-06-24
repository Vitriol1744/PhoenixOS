# Copy the relevant files over.
cp -v build/Kernel/PhoenixOS.elf limine.cfg ThirdParty/limine/limine-bios.sys \
      ThirdParty/limine/limine-bios-cd.bin ThirdParty/limine/limine-uefi-cd.bin iso_root/

# Create the EFI boot tree and copy Limine's EFI executables over.
mkdir -p iso_root/EFI/BOOT
cp -v ThirdParty/limine/BOOT*.EFI iso_root/EFI/BOOT/

KERNEL_IMAGE="build/PhoenixOS.iso"

# Create the bootable ISO.
xorriso -as mkisofs -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o $KERNEL_IMAGE

# Install Limine stage 1 and 2 for legacy BIOS boot.
./ThirdParty/limine/limine bios-install $KERNEL_IMAGE