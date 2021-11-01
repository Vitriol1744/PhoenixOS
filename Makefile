KERNEL := myos.elf

BUILD_DIR := ./build
SRC_DIRS := ./src
 
CC  = clang
CXX = clang++
AS  = nasm
 
CCFLAGS = -Wall -Wextra -pipe
ASMFLAGS = -f elf64
QEMU_FLAGS = -d int -d cpu_reset -M q35,smm=off -no-shutdown -no-reboot -monitor stdio -serial file:PhOS-logs.txt -D ./qemu-logs.txt

INTERNALLDFLAGS :=     \
	-fno-pic -no-pie\
	-Wl,-static,--no-dynamic-linker,-ztext \
	-nostdlib -nodefaultlibs      \
	-Tlinker.ld    \
	-z max-page-size=0x1000

INTERNALCCFLAGS :=     \
	-I.                  \
	-std=c17           	 \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fno-pie       \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone		 \
	-Ithird_party/stivale \
	-Isrc/userland/libc  \
	-Isrc/PhOS		     \
	-DDEBUG 
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.asm')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
 
.PHONY: all clean

all: $(BUILD_DIR)/$(KERNEL)
	cp -v $(BUILD_DIR)/$(KERNEL) limine.cfg third_party/limine/limine.sys \
		third_party/limine/limine-cd.bin third_party/limine/limine-eltorito-efi.bin iso_root/
	
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o image.iso
	
	./third_party/limine/limine-install image.iso

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
	qemu-system-x86_64 $(QEMU_FLAGS) -cdrom image.iso

rebuild:
	make clean
	make all