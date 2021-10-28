KERNEL := myos.elf

BUILD_DIR := ./build
SRC_DIRS := ./src
 
#TODO: Shouldn't use absolute paths!
CC = ~/dev-tools/x86_64elfgcc/bin/x86_64-elf-gcc
CXX = ~/dev-tools/x86_64elfgcc/bin/x86_64-elf-g++
AS = nasm
 
CXXFLAGS = -Wall -Wextra -O2 -pipe
CCFLAGS = -Wall -Wextra -pipe
ASMFLAGS = -f elf64
 
INTERNALLDFLAGS :=     \
	-fno-pic -fpie \
	-Wl,-static,-pie,--no-dynamic-linker,-ztext \
	-static-pie    \
	-nostdlib      \
	-Tlinker.ld    \
	-z max-page-size=0x1000

INTERNALCXXFLAGS  :=           \
	-I.                  \
	-std=c++20           \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fpie       \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone		 \
	-IThirdParty/stivale \
	-Isrc/userland/libc  \
	-Isrc/kernel		 \
	-DDEBUG 
INTERNALCCFLAGS :=     \
	-I.                  \
	-std=c17           	 \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fpie       \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone		 \
	-IThirdParty/stivale \
	-Isrc/userland/libc  \
	-Isrc/PhOS		     \
	-DDEBUG 
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.asm')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
 
.PHONY: all clean

all: $(BUILD_DIR)/$(KERNEL)
	cp -v $(BUILD_DIR)/$(KERNEL) limine.cfg ThirdParty/limine/limine.sys \
		ThirdParty/limine/limine-cd.bin ThirdParty/limine/limine-eltorito-efi.bin iso_root/
	
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o image.iso
	
	./ThirdParty/limine/limine-install image.iso

$(BUILD_DIR)/$(KERNEL): $(OBJS)
	$(CC) $(INTERNALLDFLAGS) $(OBJS) -o $@
	echo Kernel built!
 
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(INTERNALCCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) $(ASMFLAGS) $< -o $@
 
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

run:
	qemu-system-x86_64 -serial stdio -cdrom image.iso -d int -d cpu_reset -m 256 -no-shutdown -no-reboot