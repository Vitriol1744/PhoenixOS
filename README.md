x86-64 Operating System written in C

## Requirements:
- nasm
- clang
- ld, lld
- make
- xorriso 
- qemu
#
## How to build?
Simply by doing:
```
make
```
### How to run?
```
make qemu
```

### Credits
Limine - Modern and lightweight bootloader featuring limine boot protokół
Github Repository - https://github.com/limine-bootloader/limine.git

## TODO:
- [ ] Set up GDT.
- [ ] Load an IDT so that exceptions and interrupts can be handled.
- [ ] Write a physical memory allocator, a good starting point is a bitmap allocator.
- [ ] Write a virtual memory manager that can map, remap and unmap pages.
- [ ] Begin parsing ACPI tables, the most important one is the MADT since it contains information about the APIC.
- [ ] Start up the other CPUs.
- [ ] Set up an interrupt controller such as the APIC.
- [ ] Configure a timer such as the Local APIC timer, the PIT, or the HPET.
- [ ] Implement a scheduler to schedule threads in order make multitasking possible.
- [ ] Design a virtual file system (VFS) and implement it. The traditional UNIX VFS works and saves headaches when porting software, but you can make your own thing too.
- [ ] Implement a simple virtual file system like a memory-only tmpfs to avoid crippling the design of your VFS too much while implementing it alongside real storage filesystems.
- [ ] Decide how to abstract devices. UNIX likes usually go for a /dev virtual filesystem containing device nodes and use ioctl() alongside standard FS calls to do operations on them.
- [ ] Get a userland going by loading executables from your VFS and running them in ring 3. Set up a way to perform system calls.
- [ ] Write a PCI driver.
- [ ] Add support for a storage medium, the easiest and most common ones are AHCI and NVMe.
