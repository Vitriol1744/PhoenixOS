srcs += files(
    'Drivers/Serial.cpp',
    'Drivers/E9Logger.cpp',
    'IDT.cpp',
    'interrupts.x86_64.asm',
    'GDT.cpp')

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