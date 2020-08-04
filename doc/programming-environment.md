# Programming environment

Keep this as simple as possible, and keep it fun (for me, that
means Emacs, rather than an IDE). Requirements:

 * Emacs integration
 * C++
 * GCC toolchain
 * Makefiles?
 * OpenOCD + GDB
 * Minimal libraries: more or less just CMSIS register definitions to
   start with; later possibly FreeRTOS as well.


## Toolchain

### Compiler

GCC (gcc-arm-none-eabi-9-2020-q2-update)

https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2

(Use this instead of AUR package, because it includes GDB as well.)

Install in `/big/opt/gcc-arm-none-eabi-9-2020-q2-update` and add
`/big/opt/gcc-arm-none-eabi-9-2020-q2-update/bin` to `PATH`.


### Programmer

OpenOCD

(available as an Arch package)


### Debugger

GDB


## Platform/libraries

CMSIS


## Build system

CMake


## Editor assistance

All in Emacs!

### IDE features

All this comes from `cmake-ide` + `rtags`:

 - Indexing/references
 - Code completion
 - Syntax highlighting

Need to set up some keybindings for `rtags`.

### Compile/flash/debug
