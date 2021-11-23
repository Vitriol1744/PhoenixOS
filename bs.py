import os
import sys
import shutil
import build_tools as bt

asm_flags = '-f elf64'
c_flags = '-Wall -Werror -Wextra -pipe'
internal_c_flags = '-I. -std=c17 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone -DDEBUG'
c_include_paths = ['third_party/stivale', 'src/userland/libc', 'src/PhOS/']
ld_flags = '-fno-pic -no-pie -Wl,-static,--no-dynamic-linker,-ztext -nostdlib -nodefaultlibs -Tlinker.ld -z max-page-size=0x1000'

qemu_flags = '-d int -d cpu_reset -m 2G -M q35,smm=off -no-shutdown -no-reboot -monitor stdio -serial file:serial.log -D ./qemu.log'

as_compiler = 'nasm'
cc_compiler = 'clang'
ld = 'clang'

kernel_binary_file = 'kernel.elf'
kernel_image_file = 'phoenixos.iso'


def setup(os):
    package_manager = ''
    packages = 'clang qemu xorriso nasm'

    additional_packages = list[str]

    if os == 'arch':
        package_manager = 'pacman -Syy'
    elif os == 'debian':
        package_manager = 'apt install'
        additional_packages.append('build-essential')

    cmd = f'sudo {package_manager} {packages} {additional_packages}'
    bt.trace(cmd)
    os.system(cmd)


def build_third_party(limine_dir: str):
    bt.trace('building limine...')
    cmd = f'make -C {limine_dir}'
    print(cmd)
    os.system(cmd)
    bt.info('limine built!')


def compile_c_file(file_to_compile: str, flags: str, filename: str):
    for include_path in c_include_paths:
        flags += f' -I{root_dir}/{include_path}'
    cmd = f'{cc_compiler} {flags} -c {file_to_compile} -o {filename}.o'
    print(cmd)
    os.system(cmd)
    print()


def compile_asm_file(file_to_compile: str, flags: str, filename: str):
    cmd = f'{as_compiler} {flags} {file_to_compile} -o {filename}.o'
    print(cmd)
    os.system(cmd)


def link_object_files(root_dir: str, build_dir: str):
    bt.trace('linking object files...')
    os.chdir(root_dir)
    if not os.path.exists('iso_root'):
        os.makedirs('iso_root')
    object_files = bt.find_files(build_dir, '.o')
    obj_files = ''
    for obj in object_files:
        obj_files += f'{obj} '
    cmd = f'{ld} {ld_flags} {obj_files} -o iso_root/{kernel_binary_file}'
    print(cmd)
    os.system(cmd)
    bt.info('finished linking obj files!')


def create_iso(root_dir: str):
    bt.trace('creating iso...')

    if not os.path.exists('iso_root'):
        os.mkdir('iso_root')
    files_to_copy = ['limine.cfg', 'third_party/limine/limine.sys',
                     'third_party/limine/limine-cd.bin', 'third_party/limine/limine-eltorito-efi.bin']

    os.chdir(root_dir)
    for file in files_to_copy:
        shutil.copy(file, 'iso_root')

    cmd = f'xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o iso_root/{kernel_image_file}'
    print()
    print(cmd)
    os.system(cmd)
    bt.info('finished creating iso!')


def build(root_dir: str, build_dir: str, src_dir: str):
    bt.trace('building PhoenixOS...')
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    build_third_party(f'{root_dir}/third_party')
    bt.compile_files(build_dir, src_dir, asm_flags, '.asm', compile_asm_file)
    bt.compile_files(build_dir, src_dir,
                     f'{c_flags} {internal_c_flags}', '.c', compile_c_file)
    link_object_files(root_dir, build_dir)
    create_iso(root_dir)
    bt.info('finished building PhoenixOS!')


def run(image_name: str):
    bt.info('running PhoenixOS...')
    cmd = f'qemu-system-x86_64 {qemu_flags} -cdrom iso_root/{image_name}'
    print(cmd)
    os.system(cmd)
    bt.info('PhoenixOS terminated!')


if __name__ == "__main__":
    root_dir = os.getcwd()
    build_dir = f'{root_dir}/build'
    src_dir = f'{root_dir}/src'

    things_to_remove = ['iso_root', 'build', 'qemu.log', 'serial.log']

    for arg_ in enumerate(sys.argv):
        arg = arg_[1]

        if arg == 'clean':
            bt.clean(root_dir, things_to_remove)
        elif arg == 'build':
            build(root_dir, build_dir, src_dir)
        elif arg == 'rebuild':
            bt.clean(root_dir, things_to_remove)
            build(root_dir, build_dir, src_dir)
        elif arg == 'make_iso':
            create_iso(root_dir, kernel_binary_file)
        elif arg == 'run':
            run(kernel_image_file)
        elif arg == 'setup_arch_based':
            setup('arch')
        elif arg == 'setup_debian_based':
            setup('debian')
        elif arg == 'build_third_party':
            build_third_party(f'{root_dir}/third_party')
        else:
            continue
        break
