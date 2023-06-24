srcs += ['KernelStart.cpp']

cxx_args = [
    '-Wall',
    '-Wextra',
    '-fno-lto',
    '-m64',
    '-march=x86-64',
    '-mabi=sysv',
    '-mno-80387',
    '-mno-mmx',
    '-mno-sse',
    '-mno-sse2',
    '-mno-red-zone',
    '-mcmodel=kernel',
    '-MMD',
    '-MP'
]

ld_args = [
    '-nostdlib',
    '-static',
    #'-z max-page-size=0x1000',
    '-T' + meson.current_source_dir() + '/linker.'+arch+'.ld'
]

args = []
kernel_elf = executable('PhoenixOS.elf', src,
    include_directories : include_directories,
    link_args : ld_args,
    install : false,
)