# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vitriol1744/dev/PhoenixOS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vitriol1744/dev/PhoenixOS

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/vitriol1744/dev/PhoenixOS/CMakeFiles /home/vitriol1744/dev/PhoenixOS//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/vitriol1744/dev/PhoenixOS/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named PhoenixOS

# Build rule for target.
PhoenixOS: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 PhoenixOS
.PHONY : PhoenixOS

# fast build rule for target.
PhoenixOS/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/build
.PHONY : PhoenixOS/fast

#=============================================================================
# Target rules for targets named setup

# Build rule for target.
setup: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 setup
.PHONY : setup

# fast build rule for target.
setup/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/setup.dir/build.make CMakeFiles/setup.dir/build
.PHONY : setup/fast

#=============================================================================
# Target rules for targets named qemu

# Build rule for target.
qemu: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 qemu
.PHONY : qemu

# fast build rule for target.
qemu/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qemu.dir/build.make CMakeFiles/qemu.dir/build
.PHONY : qemu/fast

Kernel/ACPI.o: Kernel/ACPI.cpp.o
.PHONY : Kernel/ACPI.o

# target to build an object file
Kernel/ACPI.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/ACPI.cpp.o
.PHONY : Kernel/ACPI.cpp.o

Kernel/ACPI.i: Kernel/ACPI.cpp.i
.PHONY : Kernel/ACPI.i

# target to preprocess a source file
Kernel/ACPI.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/ACPI.cpp.i
.PHONY : Kernel/ACPI.cpp.i

Kernel/ACPI.s: Kernel/ACPI.cpp.s
.PHONY : Kernel/ACPI.s

# target to generate assembly for a file
Kernel/ACPI.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/ACPI.cpp.s
.PHONY : Kernel/ACPI.cpp.s

Kernel/Arch/x86/APIC.o: Kernel/Arch/x86/APIC.cpp.o
.PHONY : Kernel/Arch/x86/APIC.o

# target to build an object file
Kernel/Arch/x86/APIC.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/APIC.cpp.o
.PHONY : Kernel/Arch/x86/APIC.cpp.o

Kernel/Arch/x86/APIC.i: Kernel/Arch/x86/APIC.cpp.i
.PHONY : Kernel/Arch/x86/APIC.i

# target to preprocess a source file
Kernel/Arch/x86/APIC.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/APIC.cpp.i
.PHONY : Kernel/Arch/x86/APIC.cpp.i

Kernel/Arch/x86/APIC.s: Kernel/Arch/x86/APIC.cpp.s
.PHONY : Kernel/Arch/x86/APIC.s

# target to generate assembly for a file
Kernel/Arch/x86/APIC.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/APIC.cpp.s
.PHONY : Kernel/Arch/x86/APIC.cpp.s

Kernel/Arch/x86/Arch.o: Kernel/Arch/x86/Arch.cpp.o
.PHONY : Kernel/Arch/x86/Arch.o

# target to build an object file
Kernel/Arch/x86/Arch.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/Arch.cpp.o
.PHONY : Kernel/Arch/x86/Arch.cpp.o

Kernel/Arch/x86/Arch.i: Kernel/Arch/x86/Arch.cpp.i
.PHONY : Kernel/Arch/x86/Arch.i

# target to preprocess a source file
Kernel/Arch/x86/Arch.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/Arch.cpp.i
.PHONY : Kernel/Arch/x86/Arch.cpp.i

Kernel/Arch/x86/Arch.s: Kernel/Arch/x86/Arch.cpp.s
.PHONY : Kernel/Arch/x86/Arch.s

# target to generate assembly for a file
Kernel/Arch/x86/Arch.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/Arch.cpp.s
.PHONY : Kernel/Arch/x86/Arch.cpp.s

Kernel/Arch/x86/CMOS.o: Kernel/Arch/x86/CMOS.cpp.o
.PHONY : Kernel/Arch/x86/CMOS.o

# target to build an object file
Kernel/Arch/x86/CMOS.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/CMOS.cpp.o
.PHONY : Kernel/Arch/x86/CMOS.cpp.o

Kernel/Arch/x86/CMOS.i: Kernel/Arch/x86/CMOS.cpp.i
.PHONY : Kernel/Arch/x86/CMOS.i

# target to preprocess a source file
Kernel/Arch/x86/CMOS.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/CMOS.cpp.i
.PHONY : Kernel/Arch/x86/CMOS.cpp.i

Kernel/Arch/x86/CMOS.s: Kernel/Arch/x86/CMOS.cpp.s
.PHONY : Kernel/Arch/x86/CMOS.s

# target to generate assembly for a file
Kernel/Arch/x86/CMOS.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/CMOS.cpp.s
.PHONY : Kernel/Arch/x86/CMOS.cpp.s

Kernel/Arch/x86/CPU.o: Kernel/Arch/x86/CPU.cpp.o
.PHONY : Kernel/Arch/x86/CPU.o

# target to build an object file
Kernel/Arch/x86/CPU.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/CPU.cpp.o
.PHONY : Kernel/Arch/x86/CPU.cpp.o

Kernel/Arch/x86/CPU.i: Kernel/Arch/x86/CPU.cpp.i
.PHONY : Kernel/Arch/x86/CPU.i

# target to preprocess a source file
Kernel/Arch/x86/CPU.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/CPU.cpp.i
.PHONY : Kernel/Arch/x86/CPU.cpp.i

Kernel/Arch/x86/CPU.s: Kernel/Arch/x86/CPU.cpp.s
.PHONY : Kernel/Arch/x86/CPU.s

# target to generate assembly for a file
Kernel/Arch/x86/CPU.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/CPU.cpp.s
.PHONY : Kernel/Arch/x86/CPU.cpp.s

Kernel/Arch/x86/GDT.o: Kernel/Arch/x86/GDT.cpp.o
.PHONY : Kernel/Arch/x86/GDT.o

# target to build an object file
Kernel/Arch/x86/GDT.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/GDT.cpp.o
.PHONY : Kernel/Arch/x86/GDT.cpp.o

Kernel/Arch/x86/GDT.i: Kernel/Arch/x86/GDT.cpp.i
.PHONY : Kernel/Arch/x86/GDT.i

# target to preprocess a source file
Kernel/Arch/x86/GDT.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/GDT.cpp.i
.PHONY : Kernel/Arch/x86/GDT.cpp.i

Kernel/Arch/x86/GDT.s: Kernel/Arch/x86/GDT.cpp.s
.PHONY : Kernel/Arch/x86/GDT.s

# target to generate assembly for a file
Kernel/Arch/x86/GDT.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/GDT.cpp.s
.PHONY : Kernel/Arch/x86/GDT.cpp.s

Kernel/Arch/x86/HPET.o: Kernel/Arch/x86/HPET.cpp.o
.PHONY : Kernel/Arch/x86/HPET.o

# target to build an object file
Kernel/Arch/x86/HPET.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/HPET.cpp.o
.PHONY : Kernel/Arch/x86/HPET.cpp.o

Kernel/Arch/x86/HPET.i: Kernel/Arch/x86/HPET.cpp.i
.PHONY : Kernel/Arch/x86/HPET.i

# target to preprocess a source file
Kernel/Arch/x86/HPET.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/HPET.cpp.i
.PHONY : Kernel/Arch/x86/HPET.cpp.i

Kernel/Arch/x86/HPET.s: Kernel/Arch/x86/HPET.cpp.s
.PHONY : Kernel/Arch/x86/HPET.s

# target to generate assembly for a file
Kernel/Arch/x86/HPET.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/HPET.cpp.s
.PHONY : Kernel/Arch/x86/HPET.cpp.s

Kernel/Arch/x86/IDT.o: Kernel/Arch/x86/IDT.cpp.o
.PHONY : Kernel/Arch/x86/IDT.o

# target to build an object file
Kernel/Arch/x86/IDT.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/IDT.cpp.o
.PHONY : Kernel/Arch/x86/IDT.cpp.o

Kernel/Arch/x86/IDT.i: Kernel/Arch/x86/IDT.cpp.i
.PHONY : Kernel/Arch/x86/IDT.i

# target to preprocess a source file
Kernel/Arch/x86/IDT.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/IDT.cpp.i
.PHONY : Kernel/Arch/x86/IDT.cpp.i

Kernel/Arch/x86/IDT.s: Kernel/Arch/x86/IDT.cpp.s
.PHONY : Kernel/Arch/x86/IDT.s

# target to generate assembly for a file
Kernel/Arch/x86/IDT.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/IDT.cpp.s
.PHONY : Kernel/Arch/x86/IDT.cpp.s

Kernel/Arch/x86/LAPIC.o: Kernel/Arch/x86/LAPIC.cpp.o
.PHONY : Kernel/Arch/x86/LAPIC.o

# target to build an object file
Kernel/Arch/x86/LAPIC.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/LAPIC.cpp.o
.PHONY : Kernel/Arch/x86/LAPIC.cpp.o

Kernel/Arch/x86/LAPIC.i: Kernel/Arch/x86/LAPIC.cpp.i
.PHONY : Kernel/Arch/x86/LAPIC.i

# target to preprocess a source file
Kernel/Arch/x86/LAPIC.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/LAPIC.cpp.i
.PHONY : Kernel/Arch/x86/LAPIC.cpp.i

Kernel/Arch/x86/LAPIC.s: Kernel/Arch/x86/LAPIC.cpp.s
.PHONY : Kernel/Arch/x86/LAPIC.s

# target to generate assembly for a file
Kernel/Arch/x86/LAPIC.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/LAPIC.cpp.s
.PHONY : Kernel/Arch/x86/LAPIC.cpp.s

Kernel/Arch/x86/PIC.o: Kernel/Arch/x86/PIC.cpp.o
.PHONY : Kernel/Arch/x86/PIC.o

# target to build an object file
Kernel/Arch/x86/PIC.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/PIC.cpp.o
.PHONY : Kernel/Arch/x86/PIC.cpp.o

Kernel/Arch/x86/PIC.i: Kernel/Arch/x86/PIC.cpp.i
.PHONY : Kernel/Arch/x86/PIC.i

# target to preprocess a source file
Kernel/Arch/x86/PIC.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/PIC.cpp.i
.PHONY : Kernel/Arch/x86/PIC.cpp.i

Kernel/Arch/x86/PIC.s: Kernel/Arch/x86/PIC.cpp.s
.PHONY : Kernel/Arch/x86/PIC.s

# target to generate assembly for a file
Kernel/Arch/x86/PIC.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/PIC.cpp.s
.PHONY : Kernel/Arch/x86/PIC.cpp.s

Kernel/Arch/x86/PIT.o: Kernel/Arch/x86/PIT.cpp.o
.PHONY : Kernel/Arch/x86/PIT.o

# target to build an object file
Kernel/Arch/x86/PIT.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/PIT.cpp.o
.PHONY : Kernel/Arch/x86/PIT.cpp.o

Kernel/Arch/x86/PIT.i: Kernel/Arch/x86/PIT.cpp.i
.PHONY : Kernel/Arch/x86/PIT.i

# target to preprocess a source file
Kernel/Arch/x86/PIT.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/PIT.cpp.i
.PHONY : Kernel/Arch/x86/PIT.cpp.i

Kernel/Arch/x86/PIT.s: Kernel/Arch/x86/PIT.cpp.s
.PHONY : Kernel/Arch/x86/PIT.s

# target to generate assembly for a file
Kernel/Arch/x86/PIT.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/PIT.cpp.s
.PHONY : Kernel/Arch/x86/PIT.cpp.s

Kernel/Arch/x86/interrupts.x86_64.o: Kernel/Arch/x86/interrupts.x86_64.asm.o
.PHONY : Kernel/Arch/x86/interrupts.x86_64.o

# target to build an object file
Kernel/Arch/x86/interrupts.x86_64.asm.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Arch/x86/interrupts.x86_64.asm.o
.PHONY : Kernel/Arch/x86/interrupts.x86_64.asm.o

Kernel/BootInfo.o: Kernel/BootInfo.cpp.o
.PHONY : Kernel/BootInfo.o

# target to build an object file
Kernel/BootInfo.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/BootInfo.cpp.o
.PHONY : Kernel/BootInfo.cpp.o

Kernel/BootInfo.i: Kernel/BootInfo.cpp.i
.PHONY : Kernel/BootInfo.i

# target to preprocess a source file
Kernel/BootInfo.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/BootInfo.cpp.i
.PHONY : Kernel/BootInfo.cpp.i

Kernel/BootInfo.s: Kernel/BootInfo.cpp.s
.PHONY : Kernel/BootInfo.s

# target to generate assembly for a file
Kernel/BootInfo.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/BootInfo.cpp.s
.PHONY : Kernel/BootInfo.cpp.s

Kernel/Drivers/Terminal.o: Kernel/Drivers/Terminal.cpp.o
.PHONY : Kernel/Drivers/Terminal.o

# target to build an object file
Kernel/Drivers/Terminal.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Drivers/Terminal.cpp.o
.PHONY : Kernel/Drivers/Terminal.cpp.o

Kernel/Drivers/Terminal.i: Kernel/Drivers/Terminal.cpp.i
.PHONY : Kernel/Drivers/Terminal.i

# target to preprocess a source file
Kernel/Drivers/Terminal.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Drivers/Terminal.cpp.i
.PHONY : Kernel/Drivers/Terminal.cpp.i

Kernel/Drivers/Terminal.s: Kernel/Drivers/Terminal.cpp.s
.PHONY : Kernel/Drivers/Terminal.s

# target to generate assembly for a file
Kernel/Drivers/Terminal.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Drivers/Terminal.cpp.s
.PHONY : Kernel/Drivers/Terminal.cpp.s

Kernel/KernelStart.o: Kernel/KernelStart.cpp.o
.PHONY : Kernel/KernelStart.o

# target to build an object file
Kernel/KernelStart.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/KernelStart.cpp.o
.PHONY : Kernel/KernelStart.cpp.o

Kernel/KernelStart.i: Kernel/KernelStart.cpp.i
.PHONY : Kernel/KernelStart.i

# target to preprocess a source file
Kernel/KernelStart.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/KernelStart.cpp.i
.PHONY : Kernel/KernelStart.cpp.i

Kernel/KernelStart.s: Kernel/KernelStart.cpp.s
.PHONY : Kernel/KernelStart.s

# target to generate assembly for a file
Kernel/KernelStart.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/KernelStart.cpp.s
.PHONY : Kernel/KernelStart.cpp.s

Kernel/Memory/KernelHeap.o: Kernel/Memory/KernelHeap.cpp.o
.PHONY : Kernel/Memory/KernelHeap.o

# target to build an object file
Kernel/Memory/KernelHeap.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/KernelHeap.cpp.o
.PHONY : Kernel/Memory/KernelHeap.cpp.o

Kernel/Memory/KernelHeap.i: Kernel/Memory/KernelHeap.cpp.i
.PHONY : Kernel/Memory/KernelHeap.i

# target to preprocess a source file
Kernel/Memory/KernelHeap.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/KernelHeap.cpp.i
.PHONY : Kernel/Memory/KernelHeap.cpp.i

Kernel/Memory/KernelHeap.s: Kernel/Memory/KernelHeap.cpp.s
.PHONY : Kernel/Memory/KernelHeap.s

# target to generate assembly for a file
Kernel/Memory/KernelHeap.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/KernelHeap.cpp.s
.PHONY : Kernel/Memory/KernelHeap.cpp.s

Kernel/Memory/PhysicalMemoryManager.o: Kernel/Memory/PhysicalMemoryManager.cpp.o
.PHONY : Kernel/Memory/PhysicalMemoryManager.o

# target to build an object file
Kernel/Memory/PhysicalMemoryManager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/PhysicalMemoryManager.cpp.o
.PHONY : Kernel/Memory/PhysicalMemoryManager.cpp.o

Kernel/Memory/PhysicalMemoryManager.i: Kernel/Memory/PhysicalMemoryManager.cpp.i
.PHONY : Kernel/Memory/PhysicalMemoryManager.i

# target to preprocess a source file
Kernel/Memory/PhysicalMemoryManager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/PhysicalMemoryManager.cpp.i
.PHONY : Kernel/Memory/PhysicalMemoryManager.cpp.i

Kernel/Memory/PhysicalMemoryManager.s: Kernel/Memory/PhysicalMemoryManager.cpp.s
.PHONY : Kernel/Memory/PhysicalMemoryManager.s

# target to generate assembly for a file
Kernel/Memory/PhysicalMemoryManager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/PhysicalMemoryManager.cpp.s
.PHONY : Kernel/Memory/PhysicalMemoryManager.cpp.s

Kernel/Memory/VirtualMemoryManager.o: Kernel/Memory/VirtualMemoryManager.cpp.o
.PHONY : Kernel/Memory/VirtualMemoryManager.o

# target to build an object file
Kernel/Memory/VirtualMemoryManager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/VirtualMemoryManager.cpp.o
.PHONY : Kernel/Memory/VirtualMemoryManager.cpp.o

Kernel/Memory/VirtualMemoryManager.i: Kernel/Memory/VirtualMemoryManager.cpp.i
.PHONY : Kernel/Memory/VirtualMemoryManager.i

# target to preprocess a source file
Kernel/Memory/VirtualMemoryManager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/VirtualMemoryManager.cpp.i
.PHONY : Kernel/Memory/VirtualMemoryManager.cpp.i

Kernel/Memory/VirtualMemoryManager.s: Kernel/Memory/VirtualMemoryManager.cpp.s
.PHONY : Kernel/Memory/VirtualMemoryManager.s

# target to generate assembly for a file
Kernel/Memory/VirtualMemoryManager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Memory/VirtualMemoryManager.cpp.s
.PHONY : Kernel/Memory/VirtualMemoryManager.cpp.s

Kernel/Scheduler/Scheduler.o: Kernel/Scheduler/Scheduler.cpp.o
.PHONY : Kernel/Scheduler/Scheduler.o

# target to build an object file
Kernel/Scheduler/Scheduler.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Scheduler/Scheduler.cpp.o
.PHONY : Kernel/Scheduler/Scheduler.cpp.o

Kernel/Scheduler/Scheduler.i: Kernel/Scheduler/Scheduler.cpp.i
.PHONY : Kernel/Scheduler/Scheduler.i

# target to preprocess a source file
Kernel/Scheduler/Scheduler.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Scheduler/Scheduler.cpp.i
.PHONY : Kernel/Scheduler/Scheduler.cpp.i

Kernel/Scheduler/Scheduler.s: Kernel/Scheduler/Scheduler.cpp.s
.PHONY : Kernel/Scheduler/Scheduler.s

# target to generate assembly for a file
Kernel/Scheduler/Scheduler.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Scheduler/Scheduler.cpp.s
.PHONY : Kernel/Scheduler/Scheduler.cpp.s

Kernel/Scheduler/scheduler.x86_64.o: Kernel/Scheduler/scheduler.x86_64.asm.o
.PHONY : Kernel/Scheduler/scheduler.x86_64.o

# target to build an object file
Kernel/Scheduler/scheduler.x86_64.asm.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Scheduler/scheduler.x86_64.asm.o
.PHONY : Kernel/Scheduler/scheduler.x86_64.asm.o

Kernel/Scheduler/userland.x86_64.o: Kernel/Scheduler/userland.x86_64.asm.o
.PHONY : Kernel/Scheduler/userland.x86_64.o

# target to build an object file
Kernel/Scheduler/userland.x86_64.asm.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Scheduler/userland.x86_64.asm.o
.PHONY : Kernel/Scheduler/userland.x86_64.asm.o

Kernel/Utility/KLibC.o: Kernel/Utility/KLibC.cpp.o
.PHONY : Kernel/Utility/KLibC.o

# target to build an object file
Kernel/Utility/KLibC.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Utility/KLibC.cpp.o
.PHONY : Kernel/Utility/KLibC.cpp.o

Kernel/Utility/KLibC.i: Kernel/Utility/KLibC.cpp.i
.PHONY : Kernel/Utility/KLibC.i

# target to preprocess a source file
Kernel/Utility/KLibC.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Utility/KLibC.cpp.i
.PHONY : Kernel/Utility/KLibC.cpp.i

Kernel/Utility/KLibC.s: Kernel/Utility/KLibC.cpp.s
.PHONY : Kernel/Utility/KLibC.s

# target to generate assembly for a file
Kernel/Utility/KLibC.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Utility/KLibC.cpp.s
.PHONY : Kernel/Utility/KLibC.cpp.s

Kernel/Utility/Logger.o: Kernel/Utility/Logger.cpp.o
.PHONY : Kernel/Utility/Logger.o

# target to build an object file
Kernel/Utility/Logger.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Utility/Logger.cpp.o
.PHONY : Kernel/Utility/Logger.cpp.o

Kernel/Utility/Logger.i: Kernel/Utility/Logger.cpp.i
.PHONY : Kernel/Utility/Logger.i

# target to preprocess a source file
Kernel/Utility/Logger.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Utility/Logger.cpp.i
.PHONY : Kernel/Utility/Logger.cpp.i

Kernel/Utility/Logger.s: Kernel/Utility/Logger.cpp.s
.PHONY : Kernel/Utility/Logger.s

# target to generate assembly for a file
Kernel/Utility/Logger.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/Utility/Logger.cpp.s
.PHONY : Kernel/Utility/Logger.cpp.s

Kernel/kernelStart.o: Kernel/kernelStart.cpp.o
.PHONY : Kernel/kernelStart.o

# target to build an object file
Kernel/kernelStart.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/kernelStart.cpp.o
.PHONY : Kernel/kernelStart.cpp.o

Kernel/kernelStart.i: Kernel/kernelStart.cpp.i
.PHONY : Kernel/kernelStart.i

# target to preprocess a source file
Kernel/kernelStart.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/kernelStart.cpp.i
.PHONY : Kernel/kernelStart.cpp.i

Kernel/kernelStart.s: Kernel/kernelStart.cpp.s
.PHONY : Kernel/kernelStart.s

# target to generate assembly for a file
Kernel/kernelStart.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/PhoenixOS.dir/build.make CMakeFiles/PhoenixOS.dir/Kernel/kernelStart.cpp.s
.PHONY : Kernel/kernelStart.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... qemu"
	@echo "... setup"
	@echo "... PhoenixOS"
	@echo "... Kernel/ACPI.o"
	@echo "... Kernel/ACPI.i"
	@echo "... Kernel/ACPI.s"
	@echo "... Kernel/Arch/x86/APIC.o"
	@echo "... Kernel/Arch/x86/APIC.i"
	@echo "... Kernel/Arch/x86/APIC.s"
	@echo "... Kernel/Arch/x86/Arch.o"
	@echo "... Kernel/Arch/x86/Arch.i"
	@echo "... Kernel/Arch/x86/Arch.s"
	@echo "... Kernel/Arch/x86/CMOS.o"
	@echo "... Kernel/Arch/x86/CMOS.i"
	@echo "... Kernel/Arch/x86/CMOS.s"
	@echo "... Kernel/Arch/x86/CPU.o"
	@echo "... Kernel/Arch/x86/CPU.i"
	@echo "... Kernel/Arch/x86/CPU.s"
	@echo "... Kernel/Arch/x86/GDT.o"
	@echo "... Kernel/Arch/x86/GDT.i"
	@echo "... Kernel/Arch/x86/GDT.s"
	@echo "... Kernel/Arch/x86/HPET.o"
	@echo "... Kernel/Arch/x86/HPET.i"
	@echo "... Kernel/Arch/x86/HPET.s"
	@echo "... Kernel/Arch/x86/IDT.o"
	@echo "... Kernel/Arch/x86/IDT.i"
	@echo "... Kernel/Arch/x86/IDT.s"
	@echo "... Kernel/Arch/x86/LAPIC.o"
	@echo "... Kernel/Arch/x86/LAPIC.i"
	@echo "... Kernel/Arch/x86/LAPIC.s"
	@echo "... Kernel/Arch/x86/PIC.o"
	@echo "... Kernel/Arch/x86/PIC.i"
	@echo "... Kernel/Arch/x86/PIC.s"
	@echo "... Kernel/Arch/x86/PIT.o"
	@echo "... Kernel/Arch/x86/PIT.i"
	@echo "... Kernel/Arch/x86/PIT.s"
	@echo "... Kernel/Arch/x86/interrupts.x86_64.o"
	@echo "... Kernel/BootInfo.o"
	@echo "... Kernel/BootInfo.i"
	@echo "... Kernel/BootInfo.s"
	@echo "... Kernel/Drivers/Terminal.o"
	@echo "... Kernel/Drivers/Terminal.i"
	@echo "... Kernel/Drivers/Terminal.s"
	@echo "... Kernel/KernelStart.o"
	@echo "... Kernel/KernelStart.i"
	@echo "... Kernel/KernelStart.s"
	@echo "... Kernel/Memory/KernelHeap.o"
	@echo "... Kernel/Memory/KernelHeap.i"
	@echo "... Kernel/Memory/KernelHeap.s"
	@echo "... Kernel/Memory/PhysicalMemoryManager.o"
	@echo "... Kernel/Memory/PhysicalMemoryManager.i"
	@echo "... Kernel/Memory/PhysicalMemoryManager.s"
	@echo "... Kernel/Memory/VirtualMemoryManager.o"
	@echo "... Kernel/Memory/VirtualMemoryManager.i"
	@echo "... Kernel/Memory/VirtualMemoryManager.s"
	@echo "... Kernel/Scheduler/Scheduler.o"
	@echo "... Kernel/Scheduler/Scheduler.i"
	@echo "... Kernel/Scheduler/Scheduler.s"
	@echo "... Kernel/Scheduler/scheduler.x86_64.o"
	@echo "... Kernel/Scheduler/userland.x86_64.o"
	@echo "... Kernel/Utility/KLibC.o"
	@echo "... Kernel/Utility/KLibC.i"
	@echo "... Kernel/Utility/KLibC.s"
	@echo "... Kernel/Utility/Logger.o"
	@echo "... Kernel/Utility/Logger.i"
	@echo "... Kernel/Utility/Logger.s"
	@echo "... Kernel/kernelStart.o"
	@echo "... Kernel/kernelStart.i"
	@echo "... Kernel/kernelStart.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

