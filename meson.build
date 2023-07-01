project('PhoenixOS', ['c', 'cpp', 'nasm'], version: 'v0.0.1', license: 'GPL-3.0',
    default_options : ['c_std=c20', 'cpp_std=c++2b', 'optimization=2'])

c = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')
as = meson.get_compiler('nasm')
ld = cxx.get_linker_id()
arch = 'x86-64'

srcs = []
if arch == 'x86-64'
    subdir('Kernel/Arch/x86_64')
else
    error('Unknown architecture: ' + arch)
endif


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
    '-std=c++2b',
], language: ['c', 'cpp'])

add_global_arguments([
    '-fno-rtti',
    '-fno-exceptions',
    '-Wno-unused-parameter',
    '-I'+meson.source_root()+'/Userland/Libraries/LibC++',
    '-I'+meson.source_root()+'/ThirdParty/libstdcxx-headers/include',
    '-DFMT_HEADER_ONLY',
    '-DFMT_STATIC_THOUSANDS_SEPARATOR=1',
    '-DFMT_USE_LONG_DOUBLE=0',
    '-DFMT_USE_DOUBLE=0',
    '-DFMT_USE_FLOAT=0',
], language : 'cpp')


cmake = import('cmake')
deps = [
    dependency('compiler_rt_builtins'),
]

inc_deps = [ ]
foreach dep : deps
    inc_deps += dep.partial_dependency(includes : true)
endforeach


subdir('Userland/Libraries/LibC')
subdir('Kernel')
