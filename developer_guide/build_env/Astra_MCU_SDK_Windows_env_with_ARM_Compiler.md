# Setup Guide for Development Environment for Arm Compiler in Windows

Content:

  - [Install AC6 Compiler](#win-ac6-install-ac6-compiler)
  - [MSYS2 Installation for Windows](#win-ac6-msys2-installation-for-windows)
  - [Ninja Installation for Windows](#win-ac6-ninja-installation-for-windows)
  - [CMake](#win-ac6-cmake)
    - [CMake-3.26.0 Download](#win-ac6-cmake-3260-download)
    - [CMake Installation](#win-ac6-cmake-installation)
  - [Python](#win-ac6-python)
    - [Python Download](#win-ac6-python-download)
    - [Python Installation](#win-ac6-python-installation)
  - [Debug steps](#win-ac6-debug-steps)
  - [Environment Variables Summary](#win-ac6-environment-variables-summary)

(win-ac6-install-ac6-compiler)=
## Install AC6 Compiler
Download and Unzip the Arm Compiler from the [**Download Arm Compiler 6.19**](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)

- Click Download Arm Compiler 6.19
- Login into your Arm account
- Under "Select Revision to Download" dropdown menu, select "r6p19-00rel0"
- Download and install "Arm Compiler for Embedded 6.19 (x86_64 Windows)" 

(win-ac6-msys2-installation-for-windows)=
## MSYS2 Installation For Windows
Follow the below steps to install MSYS2 in Windows:

- Download the Windows installer for MSYS2:[**MSYS2 Download**](https://www.msys2.org/)
- Once the installer is downloaded, double-click the .exe file to start the installation process and follow the on-screen instructions in the installation wizard.
- Select "Launch MSYS2 UCRT64".
- Enter the command "pacman -S make". Enter "Y", incase prompted to.
- Add the following path: `C:\msys64\usr\bin` to Path in System Variables.

(win-ac6-ninja-installation-for-windows)=
## Ninja Installation For Windows
Follow the below steps to install ninja in Windows:

- Download and extract the zip file for ninja in Windows:[**ninja Download**](https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip)
- Add the following path: `C:\Users\user_name\Downloads\ninja-win` to Path in System Variables.

(win-ac6-cmake)=
## CMake
(win-ac6-cmake-3260-download)=
### CMake-3.26.0 Download

Download the CMake installer from the [**cmake installer**](https://github.com/Kitware/CMake/releases/download/v3.26.0-rc4/cmake-3.26.0-rc4-windows-x86_64.msi).


(win-ac6-cmake-installation)=
### CMake Installation

Run the downloaded msi file.
> Note: For Win64, enable the install option *Add CMake to the system PATH*.-

(win-ac6-python)=
## Python
(win-ac6-python-download)=
### Python Download
(Recommended 3.10 and above) 

Go to the official Python website: [**Download_python**](https://www.python.org/downloads/)

Click on "Download Python 3.x.x" (the latest stable release).

(win-ac6-python-installation)=
### Python Installation
Locate the downloaded .exe file and run it.
  
Make sure to check the box that says "Add Python to PATH" at the bottom of the installer window before clicking "Install Now". This is essential for accessing Python from the command line.

(win-ac6-debug-steps)=
### Debug steps

For debug capabilities - download the Windows installer from the link below:[**Ozone Download**](https://www.segger.com/downloads/jlink/#Ozone)

Once the installer (Ozone_Windows_V334a_x64.exe) is downloaded, double-click the .exe file to start the installation process and follow the on-screen instructions in the installation wizard.
 
(win-ac6-environment-variables-summary)=
### Environment Variables Summary

The various tools use the following environment variables.

Environment Variable     | Description
:------------------------|:------------
**AC6_TOOLCHAIN_6_22_0** | Arm Compiler path: (`C:\Program Files\ArmCompilerforEmbedded6.22\bin`)
**ARMLMD_LICENSE_FILE**  | 
