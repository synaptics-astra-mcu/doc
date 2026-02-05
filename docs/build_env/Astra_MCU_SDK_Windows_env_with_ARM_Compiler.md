# Setup Guide for Development Environment for Arm Compiler in Windows

Content:

- [Setup Guide for Development Environment for Arm Compiler in windows](#setup-guide-for-development-environment-for-arm-compiler-in-windows)
  - [Install AC6 Compiler](#install-ac6-compiler)
  - [MSYS2 Installation for Windows](#msys2-installation-for-windows)
  - [Ninja Installation for Windows](#ninja-installation-for-windows)
  - [CMake](#cmake)
    - [CMake-3.26.0 Download](#cmake-3260-download)
    - [CMake Installation](#cmake-installation)
  - [Python](#python)
    - [Python Installation](#python-installation)
  - [Debug steps](#debug-steps)
  - [Environment Variables Summary](#environment-variables-summary)

## Install AC6 Compiler
Download and Unzip the Arm Compiler from the [**Download Arm Compiler 6.19**](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)

- Click Download Arm Compiler 6.19
- Login into your Arm account
- Under "Select Revision to Download" dropdown menu, select "r6p19-00rel0"
- Download and install "Arm Compiler for Embedded 6.19 (x86_64 Windows)" 

## MSYS2 Installation For Windows
Follow the below steps to install MSYS2 in Windows:

- Download the Windows installer for MSYS2:[**MSYS2 Download**](https://www.msys2.org/)
- Once the installer is downloaded, double-click the .exe file to start the installation process and follow the on-screen instructions in the installation wizard.
- Select "Launch MSYS2 UCRT64".
- Enter the command "pacman -S make". Enter "Y", incase prompted to.
- Add the following path: `C:\msys64\usr\bin` to Path in System Variables.

## Ninja Installation For Windows
Follow the below steps to install ninja in Windows:

- Download and extract the zip file for ninja in Windows:[**ninja Download**](https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-win.zip)
- Add the following path: `C:\Users\user_name\Downloads\ninja-win` to Path in System Variables.

## CMake
### CMake-3.26.0 Download

Download the CMake installer from the [**cmake installer**](https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-windows-x86_64.msi).


### CMake Installation

Run the downloaded msi file.
> Note: For Win64, enable the install option *Add CMake to the system PATH*.-

## Python
### Python Installation

Download and run the Python installer from: [**Download_python**](https://www.python.org/ftp/python/3.13.7/python-3.13.7-amd64.exe)
  
Make sure to check the box that says "Add Python to PATH" at the bottom of the installer window before clicking "Install Now". This is essential for accessing Python from the command line.

### Debug steps

For debug capabilities - download the Windows installer from the link below:[**Ozone Download**](https://www.segger.com/downloads/jlink/#Ozone)

Once the installer (Ozone_Windows_V334a_x64.exe) is downloaded, double-click the .exe file to start the installation process and follow the on-screen instructions in the installation wizard.
 
### Environment Variables Summary

The various tools use the following environment variables.

Environment Variable     | Description
:------------------------|:------------
**AC6_TOOLCHAIN_6_22_0** | Arm Compiler path: (`C:\Program Files\ArmCompilerforEmbedded6.22\bin`)
**ARMLMD_LICENSE_FILE**  | 
