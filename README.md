Modern Unix-like Operating System written in C++
## Supported architectures
- x86-64

## Requirements:
- nasm
- clang
- ld, lld
- cmake
- make
- xorriso 
- qemu
#
## How to build?
first run this to generate cmake cache
```
cmake `.
```
next run this to update dependencies
```
cmake --build . --target setup
```
and finally you can build
```
cmake --build .
```
### How to run?
```
cmake --build . --target qemu
```

### References
Limine boot protocol - https://github.com/limine-bootloader/limine/blob/trunk/PROTOCOL.md

### Credits
Limine - Modern and lightweight bootloader featuring limine boot protocol,
Github Repository - https://github.com/limine-bootloader/limine.git

## TODO:
If you wanna see the full to-do list just checkout the trello board - https://trello.com/b/QUZDEWrp/phoenixos

- [ ] Begin parsing ACPI tables, the most important one is the MADT since it contains information about the APIC.
- [ ] Configure a timer such as the Local APIC timer, the PIT, or the HPET.
- [ ] Implement a scheduler to schedule threads in order make multitasking possible.
- [ ] Design a virtual file system (VFS) and implement it. The traditional UNIX VFS works and saves headaches when porting software, but you can make your own thing too.
- [ ] Implement a simple virtual file system like a memory-only tmpfs to avoid crippling the design of your VFS too much while implementing it alongside real storage filesystems.
- [ ] Decide how to abstract devices. UNIX likes usually go for a /dev virtual filesystem containing device nodes and use ioctl() alongside standard FS calls to do operations on them.
- [ ] Get a userland going by loading executables from your VFS and running them in ring 3. Set up a way to perform system calls.
Write a PCI driver.
- [ ] Add support for a storage medium, the easiest and most common ones are AHCI and NVMe.*/
