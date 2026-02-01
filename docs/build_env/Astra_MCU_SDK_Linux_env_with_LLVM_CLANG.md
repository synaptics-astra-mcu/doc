# Setup Guide for Development Environment on Linux for LLVM/CLANG 

Content:

- [Setup Guide for Development Environment on Linux for LLVM/CLANG](#setup-guide-for-development-environment-on-linux-for-llvm)
  - [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
  - [Install CMake](#install-cmake)
  - [Install Ninja Build System](#install-ninja-build-system)
  - [Install the ARM GNU Toolchain](#install-the-arm-gnu-toolchain)
  - [Install the LLVM Toolchain v21.1.3](#install-the-llvm-toolchain)
  - [Debug Steps](#debug-steps)

## Install Basic Tools and Dependencies
First, update your package manager and install essential tools required for building and managing software projects:

```bash
sudo apt-get update -y && apt-get -y install git wget make python3 
sudo apt-get update -y && apt-get -y install zip unzip python-is-python3
```

## Install CMake
Download and install CMake version 3.26. CMake is vital for configuring, generating, and managing build processes in a platform-independent manner:

```bash
wget https://github.com/Kitware/CMake/releases/download/v3.26.0-rc4/cmake-3.26.0-rc4-linux-x86_64.sh
sudo bash ./cmake-3.26.0-rc4-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/usr/local
```

## Install Ninja Build System
Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds:

```bash
wget https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip
unzip ninja-linux.zip
sudo cp ninja /opt/cmsis-toolbox-linux-amd64/bin/
sudo chmod a+x /opt/cmsis-toolbox-linux-amd64/bin/*
```

## Install the ARM GNU Toolchain
```bash
wget -O gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz
mkdir -p /opt/gcc-arm-none-eabi
sudo apt-get -y install xz-utils tar
tar -xf gcc-arm-none-eabi.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi
export GCC_TOOLCHAIN_ROOT=/opt/gcc-arm-none-eabi
```

## Install the LLVM Toolchain
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 21
sudo mkdir -p /opt/llvm/bin
sudo ln -sf /usr/bin/clang-21 /opt/llvm/bin/clang
sudo ln -sf /usr/bin/clang++-21 /opt/llvm/bin/clang++
export PATH=/opt/llvm/bin:$PATH
export LLVM_TOOLCHAIN_ROOT=/opt/llvm/bin
```

## Debug Steps
For debug capabilities - download debian file of Ozone debugger from: [Ozone Debugger](https://www.segger.com/downloads/jlink/#Ozone)

Then install it using the commands below:
```bash
sudo apt install build-essential
sudo dpkg --install Ozone_Linux_V334_x86_64.deb
```

> Note: LLVM compiler support is currently available only for SR110.