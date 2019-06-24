# MiniOS
Mini operating system built in ARM

## Features
* Command line interface which supports the following:
  * pwd - Prints current work directory
  * cd - Changes directory
  * ls - List contents of directory (Windows dir command format)
  * cat - Display contents of a file
  * hexdump - Display bytes of a file in hex
  * clear - Clear the display
  * [filename] - Attempts to run a executable (.bin) that is compiled and loaded into the SD card (must be the same compiler that the OS used)
* System Calls
* Framebuffer (Double buffering in the future)

## Building and Running
Note: These build instructions and provided Makefile will only work on Windows!

To build and run this project you will need to:
1. Install QEMU and GNU Arm Embedded Toolchain from the following links:
  * [QEMU](https://www.qemu.org/download/)
  * [GNU Arm Embedded Toolchain 32](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
2. Add both QEMU and the GNU Arm Embedded Toolchain to the Windows environment variables
