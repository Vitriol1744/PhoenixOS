IMAGE_ISO := Drive.iso
KERNEL_ELF := PhoenixOS.elf
LINKER_FILE := linker.ld

SRC_DIR := ./Kernel
BUILD_DIR := ./build

CXX := clang++
AS := nasm
LD := ld

CXX_DEFINES := PH_DEBUG PH_ARCH=PH_ARCH_X86_64 PH_ENABLE_LOGGING=true
CXX_DEF_FLAGS := $(addprefix -D,$(CXX_DEFINES))

CXX_INC_DIRS := limine Kernel
CXX_INC_FLAGS := $(addprefix -I,$(CXX_INC_DIRS))

CXX_WARN_LEVELS := all extra pedantic
CXX_WARN_FLAGS := $(addprefix -W,$(CXX_WARN_LEVELS))

INTERNAL_CXX_FLAGS := -MMD -std=c++20 -ffreestanding -nostdlib -mcmodel=kernel -fno-pic -fno-lto -fno-stack-protector -fno-stack-check -mno-red-zone -mno-sse -mno-sse2 -mno-mmx -mno-80387 -fno-exceptions -fno-rtti
CXX_FLAGS := $(CXX_DEF_FLAGS) $(CXX_INC_FLAGS) $(CXX_WARN_FLAGS) $(INTERNAL_CXX_FLAGS) -mabi=sysv -march=x86-64 -target x86_64-none -m64 -masm=intel
AS_FLAGS := $(AS_INC_FLAGS) -felf64
LD_FLAGS := -nostdlib -no-pie -melf_x86_64 -static -z max-page-size=0x1000 -s -T $(LINKER_FILE)

QEMUFLAGS := -M q35,smm=off -net none -smp 4 -d int,guest_errors -d cpu_reset -m 2G -no-shutdown -no-reboot -debugcon file:debug.log -D ./qemu.log -device isa-debug-exit
QEMU_RELEASE_FLAGS := -enable-kvm -cpu host -serial stdio
# flags we use during development when we don't need to debug anything
QEMU_DEV_FLAGS := -serial stdio
# flags we use when we wanna debug
QEMU_DEBUG_FLAGS := -serial file:serial.log -monitor stdio
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.asm')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(IMAGE_ISO): $(BUILD_DIR)/$(KERNEL_ELF) limine/limine-deploy
	mkdir -p $(BUILD_DIR)/iso_root
	cp -v $(BUILD_DIR)/$(KERNEL_ELF) limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin $(BUILD_DIR)/iso_root
	xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin --efi-boot-part --efi-boot-image --protective-msdos-label $(BUILD_DIR)/iso_root -o $@
	./limine/limine-deploy $(BUILD_DIR)/$(IMAGE_ISO)

$(BUILD_DIR)/$(KERNEL_ELF): $(OBJS) linker.ld
	$(LD) $(OBJS) $(LD_FLAGS) -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) $< -o $@

limine/limine-deploy:
	make -C limine

debug:
	qemu-system-x86_64 $(QEMUFLAGS) $(QEMU_DEBUG_FLAGS) $(BUILD_DIR)/$(IMAGE_ISO)

qemu:
	qemu-system-x86_64 $(QEMUFLAGS) $(QEMU_DEV_FLAGS) $(BUILD_DIR)/$(IMAGE_ISO)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
