KERNEL_ELF := kernel.elf
IMAGE_FILE := image.hdd
LINKER_FILE := linker.ld

BUILD_DIR := ./build
SRC_DIR := ./kernel/src

CC := clang
AS := nasm
LD := ld

INC_DIRS := $(SRC_DIR) $(SRC_DIR)/
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -m32 -ffreestanding -nostdlib -fno-pic -fno-stack-protector -fshort-wchar -mno-red-zone -masm=intel -mno-sse -mno-sse2 -mno-mmx -mno-80387
ASFLAGS := $(INC_FLAGS) -felf32
LDFLAGS := -nostdlib -no-pie -melf_i386 -s -T linker.ld
QEMUFLAGS := -debugcon file:logs.txt

SRCS := $(shell find $(SRC_DIR)/ -name '*.c' -or -name '*.asm')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(IMAGE_FILE): $(BUILD_DIR)/$(KERNEL_ELF)	
	dd if=/dev/zero of=$@ bs=4M count=128
	parted -s $@ mklabel msdos
	parted -s $@ mkpart primary 2048s 100%

# 	# format the image with echfs file system
	echfs-utils -m -p0 $@ quick-format 512
	
#	# copy only 446 bytes so we will not overwrite the partition table
	dd if=$(BUILD_DIR)/bootloader.bin of=$@ conv=notrunc bs=446 count=1
	
#	# copy boot signature
	dd if=$(BUILD_DIR)/bootloader.bin of=$@ conv=notrunc bs=1 count=2 skip=510 seek=510
	
#	# and finally our bootloader stage2 :)
	dd if=$(BUILD_DIR)/bootloader.bin of=$@ conv=notrunc bs=1 count=16384 skip=512 seek=512

	echfs-utils -m -p0 -v -f $@ import $< $(KERNEL_ELF)

$(BUILD_DIR)/$(KERNEL_ELF): $(OBJS) $(LINKER_FILE) $(BUILD_DIR)/bootloader.bin
	$(LD) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/bootloader.bin:
	cd boot && $(MAKE)
	cp boot/build/bootloader.bin $(BUILD_DIR)

rebuild:
	$(MAKE) clean
	$(MAKE)

rebuild_run:
	$(MAKE) clean
	$(MAKE)
	$(MAKE) qemu

build_run: $(BUILD_DIR)/$(IMAGE_FILE)
	$(MAKE) qemu

qemu:
	qemu-system-x86_64 $(QEMUFLAGS) -drive format=raw,file=$(BUILD_DIR)/$(IMAGE_FILE)

#TODO: it could be good idea to generate bochs_config using makefile
bochs: bochs_config
	bochs -f bochs_config

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	cd boot && $(MAKE) clean