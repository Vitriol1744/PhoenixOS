
IMAGE_ISO := Drive.iso
KERNEL_ELF := PhoenixOS.elf

TARGET_ARCH := x86_64
LINKER_FILE := linker.$(TARGET_ARCH).ld

SRC_DIR := ./Kernel
BUILD_DIR := ./build

CXX := clang++
AS := nasm
LD := clang

CXX_DEFINES := PH_DEBUG PH_ENABLE_LOGGING=true
CXX_DEFFLAGS := $(addprefix -D,$(CXX_DEFINES))

CXX_INCDIRS := limine Kernel
CXX_INCFLAGS := $(addprefix -I,$(CXX_INCDIRS))

CXX_WARNLEVELS := all extra pedantic
CXX_WARNFLAGS := $(addprefix -W,$(CXX_WARNLEVELS))

INTERNAL_CXXFLAGS := -MMD -std=c++20 -ffreestanding -nostdlib -mcmodel=kernel -fno-pic -fno-pie -fno-lto -fno-stack-check -mno-red-zone -mno-sse -mno-sse2 -mno-mmx -mno-80387 -fno-exceptions -fno-rtti
CXXFLAGS := $(INTERNAL_CXXFLAGS) $(CXX_DEFFLAGS) $(CXX_INCFLAGS) $(CXX_WARNFLAGS) -masm=intel -fno-stack-protector 
ASFLAGS := $(AS_INCFLAGS)
INTERNAL_LDFLAGS := -nostdlib -no-pie
LDFLAGS := $(INTERNAL_LDFLAGS) -static -z max-page-size=0x1000 -s -T $(LINKER_FILE)

QEMUFLAGS := -M q35,smm=off -net none -smp 4 -d int,guest_errors -d cpu_reset -m 128M -no-shutdown -no-reboot -debugcon file:debug.log -D ./qemu.log -device isa-debug-exit
QEMU_RELEASE_FLAGS := -enable-kvm -cpu host -serial stdio
# flags we use during development when we don't need to debug anything
QEMU_DEV_FLAGS := -serial stdio
# flags we use when we wanna debug
QEMU_DEBUG_FLAGS := -serial file:serial.log -monitor stdio

SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.$(TARGET_ARCH).asm' )
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

ifeq ($(TARGET_ARCH), x86_64)
	CXXFLAGS += -m64 -mabi=sysv -march=x86-64 -target x86_64-unknown-none-sysv -DPH_ARCH=PH_ARCH_X86_64 
	LDFLAGS += -target x86_64-none
	ASFLAGS += -felf64
endif

ifeq ($(TARGET_ARCH), ia32)
	CXXFLAGS += -m32 -mabi=sysv -march=i386 -target i386-none -DPH_ARCH=PH_ARCH_IA32
	LDFLAGS += -target i386-none
	ASFLAGS += -felf32
endif

ifeq ($(TARGET_ARCH), aarch64)
	CXXFLAGS += -m64 -target aarch64-none-elf -DP_ARCH=PH_ARCH_AARCH64
	LDFLAGS += -target aarch64-none
	ASFLAGS += -felf64
endif

$(BUILD_DIR)/$(IMAGE_ISO): $(BUILD_DIR)/$(KERNEL_ELF) limine/limine-deploy
	mkdir -p $(BUILD_DIR)/iso_root
	cp -v $(BUILD_DIR)/$(KERNEL_ELF) limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin $(BUILD_DIR)/iso_root
	xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin --efi-boot-part --efi-boot-image --protective-msdos-label $(BUILD_DIR)/iso_root -o $@
	./limine/limine-deploy $(BUILD_DIR)/$(IMAGE_ISO)

$(BUILD_DIR)/$(KERNEL_ELF): $(OBJS) $(LINKER_FILE)
	$(LD) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

limine/limine-deploy:
	make -C limine

debug:
	qemu-system-x86_64 $(QEMUFLAGS) $(QEMU_DEBUG_FLAGS) $(BUILD_DIR)/$(IMAGE_ISO)

qemu:
	qemu-system-x86_64 $(QEMUFLAGS) $(QEMU_DEV_FLAGS) $(BUILD_DIR)/$(IMAGE_ISO)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)