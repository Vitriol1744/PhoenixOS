project('PhoenixOS', ['c', 'cpp'], version: 'v0.0.1', license: 'GPL-3.0',
    default_options : ['c_std=c20', 'cpp_std=c++20', 'optimization=2'])

c = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')
ld = cxx.get_linker_id()
arch = 'x86-64'

if arch != 'x86-64'
    error('Unknown architecture: ' + arch)
endif

srcs = ['ThirdParty/flanterm/flanterm.c', 'ThirdParty/flanterm/backends/fb.c']

include_directories = [
    include_directories('ThirdParty/limine'),
    include_directories('Kernel'),
    include_directories('ThirdParty/flanterm'),
]

add_global_arguments([
    '-ffreestanding',
    '-fno-stack-protector',
    '-fno-stack-check',
    '-fno-omit-frame-pointer',
    '-fno-pic',
    '-fno-pie',
], language: ['c', 'cpp'])

add_global_arguments([
    '-fno-rtti',
    '-fno-exceptions',
    '-fsized-deallocation',
    '-Wno-unused-parameter',
    '-Wno-non-virtual-dtor',
    '-Wno-frame-address',
], language : 'cpp')



src = files('Kernel/KernelStart.cpp')

subdir('Kernel')