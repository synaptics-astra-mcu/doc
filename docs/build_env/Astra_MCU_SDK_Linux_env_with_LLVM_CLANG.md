# Setup Guide for Development Environment on Linux for LLVM/Clang

## Table of Contents
- [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
- [Install CMake](#install-cmake)
- [Install Ninja Build System](#install-ninja-build-system)
- [Install Python](#install-python)
- [Install the Arm GNU Toolchain](#install-the-arm-gnu-toolchain)
- [Install the LLVM Toolchain](#install-the-llvm-toolchain)
- [Install OpenOCD](#install-openocd)
- [Debug Steps](#debug-steps)

## Install Basic Tools and Dependencies
First, update your package manager and install essential tools required for building and managing software projects:

```bash
sudo apt-get update -y && sudo apt-get -y install git wget make python3 zip unzip python-is-python3
```

## Install CMake
Download CMake 4.1.2 for Linux. CMake is vital for configuring, generating, and managing build processes in a platform-independent manner:

```bash
wget https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-linux-x86_64.sh
sudo bash ./cmake-4.1.2-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/usr/local
```

## Install Ninja Build System
Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds:

```bash
wget https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-linux.zip
unzip ninja-linux.zip
mkdir /opt/ninja
mkdir /opt/ninja/bin
sudo cp ninja /opt/ninja/bin/
sudo chmod a+x /opt/ninja/bin/*
```
Add Ninja to PATH:
```bash
export PATH=/opt/ninja/bin:$PATH
```
Make it permanent:
```bash
echo 'export PATH=/opt/ninja/bin:$PATH' >> ~/.bashrc
```

## Install Python
Python 3.13.x or newer is required to run configuration tools (menuconfig, kconfig) and to execute scripts used during SDK build and setup.

Choose **one** of the options below:

### Option A: System Python (if already 3.13.x+)
```bash
python3 --version
```

### Option B: pyenv (recommended if you need to install Python 3.13.x)
```bash
sudo apt update
sudo apt install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev \
libnss3-dev libssl-dev libreadline-dev libffi-dev wget curl

curl https://pyenv.run | bash
export PATH="$HOME/.pyenv/bin:$PATH"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"

pyenv install 3.13.7
pyenv local 3.13.7
python3 --version
```

Optional: make pyenv available in future shells (skip this if you prefer to enable pyenv manually per shell):

```bash
echo 'export PATH="$HOME/.pyenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(pyenv init -)"' >> ~/.bashrc
echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.bashrc
```

If you chose the optional lines above, restart your shell or run `source ~/.bashrc` to apply them.

## Install the Arm GNU Toolchain
```bash
wget -O gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz
mkdir -p /opt/gcc-arm-none-eabi
sudo apt-get -y install xz-utils tar
tar -xf gcc-arm-none-eabi.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi
export PATH=/opt/gcc-arm-none-eabi/bin:$PATH
export GCC_TOOLCHAIN_ROOT=/opt/gcc-arm-none-eabi
```
Make it permanent:
```bash
echo 'export PATH=/opt/gcc-arm-none-eabi/bin:$PATH' >> ~/.bashrc
echo 'export GCC_TOOLCHAIN_ROOT=/opt/gcc-arm-none-eabi' >> ~/.bashrc
```

Note: LLVM builds require `GCC_TOOLCHAIN_ROOT` for the GCC sysroot and libstdc++.

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
Make it permanent:
```bash
echo 'export PATH=/opt/llvm/bin:$PATH' >> ~/.bashrc
echo 'export LLVM_TOOLCHAIN_ROOT=/opt/llvm/bin' >> ~/.bashrc
```

## Install OpenOCD

```bash
sudo apt-get update
sudo apt-get install -y openocd
openocd --version
```

## Debug Steps
For debug capabilities - download debian file of Ozone debugger from: [Ozone Debugger](https://www.segger.com/downloads/jlink/#Ozone)

Then install it using the commands below:
```bash
sudo apt install build-essential
sudo dpkg --install Ozone_Linux_V334_x86_64.deb
```

> Note: LLVM compiler support is currently available only for SR110.