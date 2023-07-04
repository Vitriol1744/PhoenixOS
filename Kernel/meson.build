srcs += files(
    'Drivers/Terminal.cpp',
    'Memory/KernelHeap.cpp',
    'Memory/PhysicalMemoryManager.cpp',
    'Memory/VirtualMemoryManager.cpp',
    'Utility/Logger.cpp',
    'Utility/Stacktrace.cpp',
    'VirtualFileSystem/DevTmpFs/DevTmpFs.cpp',
    'VirtualFileSystem/DevTmpFs/DevTmpFsINode.cpp',
    'VirtualFileSystem/Initrd/Initrd.cpp',
    'VirtualFileSystem/Initrd/Ustar.cpp',
    'VirtualFileSystem/TmpFs/TmpFs.cpp',
    'VirtualFileSystem/TmpFs/TmpFsINode.cpp',
    'VirtualFileSystem/INode.cpp',
    'VirtualFileSystem/VirtualFileSystem.cpp',
    'BootInfo.cpp',
    'icxxabi.cpp',
    'KernelStart.cpp',)

cxx_args = [
    #'-g',
    #'-O2',
    #'-Wall',
    #'-Wextra',
    #'-Wpedantic',
    #'-fno-lto',
    #'-m64',
    '-mno-red-zone',

]

ld_args = [
    #'-g',
    '-nostdlib',
    #'-no-pie',
    '-static',
    #'--rtlib=compiler-rt',
    '-fuse-ld=lld',
    '-Wl,-z,max-page-size=0x1000',
    '-Wl,-T,' + meson.current_source_dir() + '/linker.'+arch+'.ld'
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
