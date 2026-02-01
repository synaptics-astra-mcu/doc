# Setup guide for development environment for LLVM/CLANG in windows

Content:

- [Setup guide for development environment for LLVM/CLANG in windows](#setup-guide-for-development-environment-for-llvm-in-windows)
  - [Install ARM GNU Toolchain](#install-arm-gnu-toolchain)
  - [Install LLVM Toolchain v21.1.0](#install-llvm-toolchain)
  - [MSYS2 Installation for Windows](#msys2-installation-for-windows)
  - [Ninja Installation for Windows](#ninja-installation-for-windows)
  - [CMake](#cmake)
    - [CMake 3.26.0 Download ](#cmake-3260-download)
    - [CMake Installation](#cmake-installation)
  - [Python](#python)
    - [Python Download](#python-download)
    - [Python Installation](#python-installation)
  - [Kconfiglib Submodule](#add-kconfiglib-submodule)
  - [Debug Steps](#debug-steps)
  - [Environment Variables Summary](#environment-variables-summary)
  
## Install ARM GNU Toolchain

Download the Arm GNU toolchain from: [**ARM_GNU toolchain Download**](https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-arm-none-eabi.exe?rev=07af46c1f7574a77969b0f764a1255f0&hash=CD4EEFA52E00B98CAC037B4936A76E56830E5248)

Run the .exe to install ARM_GNU toolchain

Add environment variable 
```bash
GCC_TOOLCHAIN_ROOT = C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1
```

## Install LLVM Toolchain

Download the LLVM Toolchain from: [**LLVM TOOLCHAIN DOWNLOAD**](https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.0/LLVM-21.1.0-win32.exe)

Run the .exe to install LLVM Toolchain

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

- Download and extract the zip file for ninja in Windows: [**ninja**](https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip)
- Add the following path: `C:\Users\user_name\Downloads\ninja-win` to Path in System Variables.

## CMake
### CMake 3.26.0 Download 

Download the CMake installer from the [**cmake installer**](https://github.com/Kitware/CMake/releases/download/v3.26.0-rc4/cmake-3.26.0-rc4-windows-x86_64.msi).

### CMake Installation

Run the downloaded msi file.
> Note: For Win64, enable the install option *Add CMake to the system PATH*.-

## Python
### Python Download 
(Recommended 3.10 and above)

Go to the official Python website: [**python**](https://www.python.org/downloads/)

Click on `Download Python 3.x.x` (the latest stable release).

### Python Installation
Locate the downloaded .exe file and run it.
  
> Make sure to check the box that says `Add Python to PATH` at the bottom of the installer window before clicking `Install Now`. This is essential for accessing Python from the command line.

### Debug steps

For debug capabilities - download the Windows installer from the link below: [**Ozone Debugger**](https://www.segger.com/downloads/jlink/#Ozone)

Once the installer `Ozone_Windows_V334a_x64.exe` is downloaded, double-click the `.exe` file to start the installation process and follow the on-screen instructions in the installation wizard.

### Environment Variables Summary

The various tools use the following environment variables.

Environment Variable     | Description
:------------------------|:------------
**GCC_TOOLCHAIN_ROOT** | GCC Compiler path: (`C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1`)
**LLVM_TOOLCHAIN_ROOT** | LLVM Compiler path: (`C:\Program Files (x86)\LLVM\bin`)

> Note: LLVM compiler support is currently available only for SR110.
