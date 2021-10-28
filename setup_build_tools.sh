sudo pacman -Syyu qemu
sudo pacman -S nasm
sudo pacman -S base-devel
sudo pacman -S gmp
sudo pacman -S libmpc
sudo pacman -S mpfr

export PREFIX="$HOME/dev-tools/x86_64elfgcc"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"
mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.35.1.tar.gz
tar xf binutils-2.35.1.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.35.1/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror -prefix=$PREFIX 2>&1 | tee configure.log
sudo make all install 2>&1 | tee make.log
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz
tar xf gcc-10.2.0.tar.gz
mkdir gcc-build
cd gcc-build
../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-language=c,c++ --without-headers
sudo make all-gcc
sudo make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
sudo rm -rf /tmp/src
