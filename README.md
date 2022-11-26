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
cmake -B build .
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
If you wanna see to-do list just checkout the trello board - https://trello.com/b/QUZDEWrp/phoenixos
