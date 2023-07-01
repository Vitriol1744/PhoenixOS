srcs += files(
    'KernelStart.cpp',
    'Drivers/Terminal.cpp',
    'BootInfo.cpp',
    'Utility/Stacktrace.cpp',
    'Utility/Logger.cpp',
    'Memory/PhysicalMemoryManager.cpp',
    'Memory/KernelHeap.cpp',
    'icxxabi.cpp')

cxx_args = [
    '-g',
    '-O2',
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-fno-lto',
    '-m64',
    '-march=x86-64',
    '-mabi=sysv',
    #'-mno-80387',
    '-mno-mmx',
    #'-target x86_64-none',
    '-mno-sse',
    '-mno-sse2',
    '-mno-red-zone',
    '-msoft-float',
    '-mcmodel=kernel'
]

ld_args = [
    '-g',
    '-nostdlib',
    '-no-pie',
    '-static',
    '--rtlib=compiler-rt',
    #'-z max-page-size=0x1000',
    '-T' + meson.current_source_dir() + '/linker.'+arch+'.ld'
]

args = []
kernel_elf = executable('PhoenixOS.elf', srcs,
    dependencies: deps,
    include_directories : include_directories,
    cpp_args: cxx_args,
    link_args : ld_args,
    install : false,
)
