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
    '-mno-red-zone',
]

ld_args = [
    '-g',
    '-nostdlib',
    '-no-pie',
    '-static',
    '--rtlib=compiler-rt',
    '-fuse-ld=lld',
    #'-z max-page-size=0x1000',
    '-T' + meson.current_source_dir() + '/linker.'+arch+'.ld'
]

if arch == 'x86_64'
    cxx_args += ['-DPH_ARCH=PH_ARCH_X86_64']
    subdir('Arch/x86_64')
elif arch == 'aarch64'
    subdir('Arch/aarch64')
    cxx_args += ['-DPH_ARCH=PH_ARCH_AARCH64']
else
    error('Unknown architecture: ' + arch)
endif

args = []
kernel_elf = executable('PhoenixOS.elf', srcs,
    dependencies: deps,
    include_directories : include_directories,
    cpp_args: cxx_args,
    link_args : ld_args,
    install : false,
)
