srcs += files(
    'Drivers/Keyboard.cpp',
    'Drivers/PIC.cpp',
    'Drivers/Serial.cpp',
    'Arch.cpp',
    'CMOS.cpp',
    'GDT.cpp',
    'IDT.cpp',
    'InterruptManager.cpp',
    'interrupts.x86_64.asm',)

cxx_args += [
    '-march=x86-64',
    '-msoft-float',
    '-mno-mmx',
    '-mno-sse',
    '-mno-sse2',
    '-mno-red-zone',
    '-mcmodel=kernel',
    '-mabi=sysv',
    '-target', 'x86_64-none',
    ]

ld_args += ['-Wl,-z,noexecstack']