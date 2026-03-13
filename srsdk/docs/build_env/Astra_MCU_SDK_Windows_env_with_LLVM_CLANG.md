# Setup Guide for Development Environment for LLVM/Clang on Windows

## Table of Contents
- [Install Arm GNU Toolchain](#install-arm-gnu-toolchain)
- [Install LLVM Toolchain](#install-llvm-toolchain)
- [MSYS2 Installation for Windows](#msys2-installation-for-windows)
- [Ninja Installation for Windows](#ninja-installation-for-windows)
- [CMake](#cmake)
  - [CMake 4.1.2 Download](#cmake-412-download)
  - [CMake Installation](#cmake-installation)
- [Python](#python)
  - [Python Download](#python-download)
  - [Python Installation](#python-installation)
- [Install OpenOCD](#install-openocd)
- [Debug Steps](#debug-steps)
- [Environment Variables Summary](#environment-variables-summary)
  
## Install Arm GNU Toolchain

Download Arm GNU Toolchain 13.2.rel1 for Windows: [**Arm GNU Toolchain**](https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-arm-none-eabi.exe?rev=07af46c1f7574a77969b0f764a1255f0&hash=CD4EEFA52E00B98CAC037B4936A76E56830E5248)

Run the `.exe` installer to install the Arm GNU Toolchain.

Add environment variable
```bash
GCC_TOOLCHAIN_ROOT = C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1
```

Note: LLVM builds require `GCC_TOOLCHAIN_ROOT` for the GCC sysroot and libstdc++.

## Install LLVM Toolchain

Download LLVM Toolchain 21.1.0 for Windows: [**LLVM Toolchain**](https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.0/LLVM-21.1.0-win32.exe)

Run the `.exe` installer to install the LLVM Toolchain.

Add environment variable
```bash
LLVM_TOOLCHAIN_ROOT = C:\Program Files (x86)\LLVM\bin
```

## MSYS2 Installation For Windows
Follow the below steps to install MSYS2 in Windows:

- Download the Windows installer for MSYS2: [**MSYS2**](https://www.msys2.org/)
- Once the installer is downloaded, double-click the `.exe` file to start the installation process and follow the on-screen instructions in the installation wizard.
- Select `Launch MSYS2 UCRT64`.
- Enter the command  `pacman -S make`. Enter `Y` , incase prompted to.
- Add the following path: `C:\msys64\usr\bin` to Path in System Variables.

## Ninja Installation For Windows
Follow the below steps to install ninja in Windows:

- Download Ninja v1.13.1 for Windows: [**Ninja**](https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-win.zip)
- Add the following path: `C:\Users\user_name\Downloads\ninja-win` to Path in System Variables.

## CMake
### CMake 4.1.2 Download

Download CMake 4.1.2 for Windows: [**CMake installer**](https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-windows-x86_64.msi).

### CMake Installation

Run the downloaded MSI file.
> Note: For Win64, enable the install option *Add CMake to the system PATH*.-

## Python
### Python Download
(Recommended 3.13.x)

Go to the official Python website: [**Python**](https://www.python.org/downloads/)

Click on `Download Python 3.13.x` (the latest 3.13 release).

### Python Installation
Locate the downloaded `.exe` file and run it.
  
> Make sure to check the box that says `Add Python to PATH` at the bottom of the installer window before clicking `Install Now`. This is essential for accessing Python from the command line.

## Install OpenOCD

OpenOCD is required for flashing and debugging over SWD/JTAG. Download a Windows build (for example xPack OpenOCD) and add its `bin` directory to your PATH.

- xPack OpenOCD: [OpenOCD xPack releases](https://github.com/xpack-dev-tools/openocd-xpack/releases)

Verify:
```bash
openocd --version
```

## Debug Steps

For debug capabilities - download the Windows installer from the link below: [**Ozone Debugger**](https://www.segger.com/downloads/jlink/#Ozone)

Once the installer `Ozone_Windows_V334a_x64.exe` is downloaded, double-click the `.exe` file to start the installation process and follow the on-screen instructions in the installation wizard.

### Environment Variables Summary

The various tools use the following environment variables.

Environment Variable     | Description
:------------------------|:------------
**GCC_TOOLCHAIN_ROOT** | GCC Compiler root: (`C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1`)
**LLVM_TOOLCHAIN_ROOT** | LLVM Compiler path: (`C:\Program Files (x86)\LLVM\bin`)

> Note: LLVM compiler support is currently available only for SR110.