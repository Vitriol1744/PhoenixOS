KERNEL := myos.elf

BUILD_DIR := ./build
SRC_DIRS := ./src
 
CXX = /usr/local/x86_64elfgcc/bin/x86_64-elf-g++
 
CXXFLAGS = -Wall -Wextra -O2 -pipe
 
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
	-Isrc/PhoenixOS      \
	-Isrc/libc			 \
	-DDEBUG
 
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c')
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
	$(CXX) $(INTERNALLDFLAGS) $(OBJS) -o $@
	echo Kernel built!
 
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INTERNALCXXFLAGS) -c $< -o $@
	echo All C Files Compiled!

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INTERNALCXXFLAGS) -c $< -o $@
	echo All CXX Files Compiled!

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

run:
	qemu-system-x86_64 -serial stdio -cdrom image.iso