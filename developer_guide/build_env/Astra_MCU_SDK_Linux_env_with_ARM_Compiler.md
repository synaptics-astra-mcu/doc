# Setup Guide for Development Environment on Linux for Arm compiler
 
Content:

  - [Install Basic Tools and Dependencies](#linux-ac6-install-basic-tools-and-dependencies)
  - [Install CMake](#linux-ac6-install-cmake)
  - [Install Ninja Build](#linux-ac6-install-ninja-build)
  - [Install the ARM Compiler](#linux-ac6-install-the-arm-compiler)
  - [Debug Steps](#linux-ac6-debug-steps)
   
(linux-ac6-install-basic-tools-and-dependencies)=
## Install Basic Tools and Dependencies
First, update your package manager and install essential tools required for building and managing software projects:
 
```bash
sudo apt-get update -y && apt-get -y install git wget make python3 
sudo apt-get update -y && apt-get -y install zip unzip python-is-python3
```
 
(linux-ac6-install-cmake)=
## Install CMake
Download and install CMake version 3.26. CMake is vital for configuring, generating, and managing build processes in a platform-independent manner:
 
```bash
wget https://github.com/Kitware/CMake/releases/download/v3.26.0-rc4/cmake-3.26.0-rc4-linux-x86_64.sh
sudo bash ./cmake-3.26.0-rc4-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/usr/local
```

(linux-ac6-install-ninja-build)=
## Install Ninja Build
Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds:
 
```bash
sudo apt-get install ninja-build
```
 
(linux-ac6-install-the-arm-compiler)=
## Install the ARM Compiler
Download ARM Compiler 6.19 from [ARM_Compiler_for_linux](https://developer.arm.com/downloads)
 
```bash
sudo mkdir -p /home/temp
sudo tar -xzf ARMCompiler6.19_standalone_linux-x86_64.tar.gz -C /home/temp
cd /home/temp
sudo ./install_x86_64.sh --i-agree-to-the-contained-eula --no-interactive -d /home/arm_compiler/
export PATH=$PATH:/home/arm_compiler/bin
export ARMLMD_LICENSE_FILE="<path_to_license_file>"
```

(linux-ac6-debug-steps)=
## Debug Steps
For debug capabilities - download debian file of Ozone debugger from the link below:
https://www.segger.com/downloads/jlink/#Ozone
Then install it using the commands below:
```bash
sudo apt install build-essential
sudo dpkg --install Ozone_Linux_V334_x86_64.deb
```
