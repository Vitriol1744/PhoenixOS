project('PhoenixOS', ['c', 'cpp', 'nasm'], version: 'v0.0.1', license: 'GPL-3.0',
    default_options : ['c_std=c17', 'cpp_std=c++2a', 'optimization=2'])

c = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')
ld = cxx.get_linker_id()
arch = host_machine.cpu_family()

srcs = []

include_directories = [
    include_directories('ThirdParty/limine'),
    include_directories('Kernel'),
    include_directories('ThirdParty/fmt/include'),
    include_directories('Userland/Libraries/LibC'),
    include_directories('Userland/Libraries/LibC++'),
    include_directories('ThirdParty/libstdcxx-headers/include'),
    include_directories('ThirdParty/smart_ptr/include'),
]

add_global_arguments([
    '-ffreestanding',
    '-fno-stack-protector',
    '-fno-stack-check',
    '-fno-pic',
    '-fno-pie',
    '-nostdlib',
    '-nostdinc++',
    '-fno-omit-frame-pointer',
    '-Ofast',
], language: ['c', 'cpp'])

if arch == 'x86_64'
add_global_arguments(['-mcmodel=kernel'], language: ['c', 'cpp'])
endif

add_global_arguments([
    '-fno-rtti',
    '-std=c++2b',
    '-fno-exceptions',
    '-Wno-unused-parameter',
    '-fsized-deallocation',
    '-Wno-non-virtual-dtor',
    '-Wno-frame-address',
    '-I'+meson.source_root()+'/Userland/Libraries/LibC++',
    '-I'+meson.source_root()+'/ThirdParty/libstdcxx-headers/include',
    '-DFMT_HEADER_ONLY',
    '-DFMT_STATIC_THOUSANDS_SEPARATOR=1',
    '-DFMT_USE_LONG_DOUBLE=0',
    '-DFMT_USE_DOUBLE=0',
    '-DFMT_USE_FLOAT=0',
], language : 'cpp')


cmake = import('cmake')
phmap_opts = cmake.subproject_options()
phmap_opts.add_cmake_defines({
    'PHMAP_BUILD_EXAMPLES' : false,
    'PHMAP_BUILD_TESTS' : false,
    'PHMAP_INSTALL' : false
})

deps = [
    cmake.subproject('parallel_hashmap', options : phmap_opts).dependency('phmap'),
    dependency('compiler_rt_builtins'),
    dependency('cwalk'),
]

inc_deps = [ ]
foreach dep : deps
    inc_deps += dep.partial_dependency(includes : true)
endforeach


subdir('Userland/Libraries/LibC')
subdir('Kernel')

qemu_args = [
    '-cpu', 'max', '-net', 'none',
    '-serial', 'stdio', '-d', 'int,guest_errors',
    '-smp', '4', '-d', 'cpu_reset',
    '-m', '2G', '-cpu', 'host',
    '-rtc', 'base=localtime',
    '-no-shutdown', '-no-reboot', '-D', './qemu.log',
    '-boot', 'order=d,menu=on,splash-time=100']
qemu = 'qemu-system-' + arch

if arch == 'x86_64'
    qemu_args += [
        '-M', 'q35,smm=off', '-device', 'isa-debug-exit',
        '-debugcon', 'file:debug.log', '-enable-kvm',
        '-M', 'accel=kvm:hvf:whpx:haxm:tcg'
        ]
        ovmf_id = 'X64'
elif arch == 'aarch64'
    qemu_args += ['-cpu', 'cortex-a72', '-M', 'virt', '-device', 'ramfb']
    ovmf_id = 'AA64'
else
    error('Unknown architecture: ' + arch)
endif

iso_dir = 'iso_root'

ksyms = custom_target('ksyms',
        input: [kernel_elf],
        output: 'ksyms.sym',
        command: [
            './generate_symbols.sh'
        ],
        build_by_default: true,
        console: true,
        install: false,
        depends: [kernel_elf]
    )

iso_files = [
        meson.source_root() / 'limine.cfg',
        meson.source_root() / 'ThirdParty/limine/limine-bios.sys',
        meson.source_root() / 'ThirdParty/limine/limine-bios-cd.bin',
        meson.source_root() / 'ThirdParty/limine/limine-uefi-cd.bin',
        ksyms,
        meson.source_root() / 'initrd.tar.gz',
    ]

iso = custom_target('iso',
    input : [kernel_elf],
    output : 'PhoenixOS.iso',
    command : [
        'mkdir', '-p', iso_dir, '&&',
        'cp', '@INPUT0@', iso_dir / 'PhoenixOS.elf', '&&',
        'cp', iso_files, iso_dir, '&&',
        'xorriso', '-as', 'mkisofs', '-b', 'limine-bios-cd.bin',
        '-no-emul-boot', '-boot-load-size', '4', '-boot-info-table',
        '--efi-boot', 'limine-uefi-cd.bin', '-efi-boot-part',
        '--efi-boot-image', '--protective-msdos-label',
        iso_dir, '-o', '@OUTPUT@', '&&',
        meson.source_root() / 'ThirdParty/limine/limine', 'bios-install', '@OUTPUT@', '&&',
        'rm', '-rf', iso_dir
    ],
    build_by_default : true,
    console : true,
    install : false,
    depends : [kernel_elf]
)

run_bios = custom_target('run_bios',
        input : iso,
        output : 'run_bios',
        command : [qemu, qemu_args, '@INPUT@'],
        console : true,
        install : false,
        depends: [iso]
    )


ovmf_binaries = subproject('ovmf_binaries')
ovmf = ovmf_binaries.get_variable(ovmf_id)

run_efi = custom_target('run_uefi',
    input : iso,
    output : 'run_uefi',
    command : [qemu, qemu_args, '-cdrom', '@INPUT@', '-bios', ovmf],
    console : true,
    install : false,
    depends : iso
)