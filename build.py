import os
import sys
import shutil
from typing import Callable

asm_flags = '-f elf64'
c_flags = '-Wall -Werror -Wextra -pipe'
internal_c_flags = '-I. -std=c17 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone -DDEBUG'
c_include_paths = ['third_party/stivale', 'src/userland/libc', 'src/PhOS/']
ld_flags = '-fno-pic -no-pie -Wl,-static,--no-dynamic-linker,-ztext -nostdlib -nodefaultlibs -Tlinker.ld -z max-page-size=0x1000'

qemu_flags = '-d int -d cpu_reset -M q35,smm=off -no-shutdown -no-reboot -monitor stdio -serial file:serial.log -D ./qemu.log'

as_compiler = 'nasm'
cc_compiler = 'clang'
ld = 'clang'

FOREGROUND_BLACK = '\u001b[30m'
FOREGROUND_RED = '\u001b[31m'
FOREGROUND_GREEN = '\u001b[32m'
FOREGROUND_YELLOW = '\u001b[33m'
FOREGROUND_BLUE = '\u001b[34m'
FOREGROUND_MAGENTA = '\u001b[35m'
FOREGROUND_CYAN = '\u001b[36m'
FOREGROUND_WHITE = '\u001b[37m'

BACKGROUND_BLACK = '\u001b[40m'
BACKGROUND_RED = '\u001b[41m'
BACKGROUND_GREEN = '\u001b[42m'
BACKGROUND_YELLOW = '\u001b[43m'
BACKGROUND_BLUE = '\u001b[44m'
BACKGROUND_MAGENTA = '\u001b[45m'
BACKGROUND_CYAN = '\u001b[46m'
BACKGROUND_WHITE = '\u001b[47m'

COLOR_RESET = '\u001b[0m'


def trace(msg: str):
    print(FOREGROUND_GREEN)
    print(msg)
    print(FOREGROUND_WHITE)


def info(msg: str):
    print(FOREGROUND_CYAN)
    print(msg)
    print(FOREGROUND_WHITE)


def error(msg: str):
    print(FOREGROUND_RED)
    print(msg)
    print(FOREGROUND_WHITE)


def clean(build_dir):
    trace('cleaning...')
    shutil.rmtree(build_dir)


def setup(os):
    package_manager = ''
    packages = 'clang qemu xorriso nasm'

    if os == 'arch':
        package_manager = 'pacman -Syy'
    elif os == 'debian':
        package_manager = 'apt install'

    cmd = f'sudo {package_manager} {packages}'
    trace(cmd)
    os.system(cmd)


def find_files(dir: str, extension: str) -> list[str]:
    ret_files = []
    for subdir, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(extension):
                ret_files.append(os.path.join(subdir, file))
    return ret_files


def compile_files(build_dir: str, src_dir: str, flags: str, extension: str, func: Callable[[str, str, str], int]):
    trace(f'compiling {extension} files')
    files_to_compile = find_files(src_dir, extension)

    for file_to_compile in files_to_compile:
        os.chdir(build_dir)
        split = os.path.split(os.path.abspath(file_to_compile))
        dir = f'./{split[0].removeprefix(src_dir)}'
        filename = split[1]
        if not os.path.exists(dir):
            os.makedirs(dir)
        os.chdir(dir)
        func(file_to_compile, flags, filename)


def compile_c_file(file_to_compile: str, flags: str, filename: str):
    for include_path in c_include_paths:
        flags += f' -I{root_dir}/{include_path}'
    cmd = f'{cc_compiler} {flags} -c {file_to_compile} -o {filename}.o'
    print(cmd)
    os.system(cmd)


def compile_asm_file(file_to_compile: str, flags: str, filename: str):
    cmd = f'{as_compiler} {flags} {file_to_compile} -o {filename}.o'
    print(cmd)
    os.system(cmd)


def link_object_files(root_dir: str, build_dir: str):
    trace('linking object files...')
    os.chdir(root_dir)
    object_files = find_files(build_dir, '.o')
    obj_files = ''
    for obj in object_files:
        obj_files += f'{obj} '
    cmd = f'{ld} {ld_flags} {obj_files} -o kernel.elf'
    print(cmd)
    os.system(cmd)


def create_iso(root_dir: str, binary_file: str):
    trace('creating iso...')
    # todo: make it cross-platform!
    cmd = f'cp -v {binary_file} limine.cfg third_party/limine/limine.sys third_party/limine/limine-cd.bin third_party/limine/limine-eltorito-efi.bin iso_root'
    os.chdir(root_dir)
    print(cmd)
    os.system(cmd)
    cmd = 'xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o image.iso'
    print()
    print(cmd)
    os.system(cmd)


def build(root_dir: str, build_dir: str, src_dir: str):
    trace('building...')
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    compile_files(build_dir, src_dir, asm_flags, '.asm', compile_asm_file)
    compile_files(build_dir, src_dir,
                  f'{c_flags} {internal_c_flags}', '.c', compile_c_file)
    link_object_files(root_dir, build_dir)
    create_iso(root_dir, 'kernel.elf')


def run(image_name: str):
    info('running...')
    cmd = f'qemu-system-x86_64 {qemu_flags} -cdrom {image_name}'
    print(cmd)
    os.system(cmd)


if __name__ == "__main__":
    root_dir = os.getcwd()
    build_dir = f'{root_dir}/build'
    src_dir = f'{root_dir}/src'

    for arg_ in enumerate(sys.argv):
        arg = arg_[1]
        if arg == 'clean':
            clean(build_dir)
        elif arg == 'build':
            build(root_dir, build_dir, src_dir)
        elif arg == 'rebuild':
            clean(build_dir)
            build(root_dir, build_dir, src_dir)
        elif arg == 'make_iso':
            create_iso(root_dir, 'kernel.elf')
        elif arg == 'run':
            run('image.iso')
        elif arg == 'setup_arch_based':
            setup('arch')
        elif arg == 'setup_debian_based':
            setup('debian')
