# Setup Guide for Development Environment on Linux for GCC 

Content:

  - [Install Basic Tools and Dependencies](#linux-gcc-install-basic-tools-and-dependencies)
  - [Install CMake](#linux-gcc-install-cmake)
  - [Install Ninja Build System](#linux-gcc-install-ninja-build-system)
  - [Install the ARM GNU GCC Compiler](#linux-gcc-install-the-arm-gnu-gcc-compiler)
  - [Debug Steps](#linux-gcc-debug-steps)

(linux-gcc-install-basic-tools-and-dependencies)=
## Install Basic Tools and Dependencies
First, update your package manager and install essential tools required for building and managing software projects:

```bash
sudo apt-get update -y && apt-get -y install git wget make python3 
sudo apt-get update -y && apt-get -y install zip unzip python-is-python3
sudo apt-get install ninja-build
```

(linux-gcc-install-cmake)=
## Install CMake
Download and install CMake version 3.26. CMake is vital for configuring, generating, and managing build processes in a platform-independent manner:

```bash
wget https://github.com/Kitware/CMake/releases/download/v3.26.0-rc4/cmake-3.26.0-rc4-linux-x86_64.sh
sudo bash ./cmake-3.26.0-rc4-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/usr/local
```

(linux-gcc-install-ninja-build-system)=
## Install Ninja Build System
Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds:

```bash
wget https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip
unzip ninja-linux.zip
sudo cp ninja /opt/cmsis-toolbox-linux-amd64/bin/
sudo chmod a+x /opt/cmsis-toolbox-linux-amd64/bin/*
```

(linux-gcc-install-the-arm-gnu-gcc-compiler)=
## Install the ARM GNU GCC Compiler
```bash
wget -O gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz
mkdir -p /opt/gcc-arm-none-eabi
sudo apt-get -y install xz-utils tar
tar -xf gcc-arm-none-eabi.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi
export PATH=$PATH:/opt/gcc-arm-none-eabi/bin
export GCC_TOOLCHAIN_13_2_1="/opt/gcc-arm-none-eabi-13.2.Rel1/bin"
```

(linux-gcc-debug-steps)=
## Debug Steps
For debug capabilities - download debian file of Ozone debugger from: [Ozone Debugger](https://www.segger.com/downloads/jlink/#Ozone)

Then install it using the commands below:
```bash
sudo apt install build-essential
sudo dpkg --install Ozone_Linux_V334_x86_64.deb
```
