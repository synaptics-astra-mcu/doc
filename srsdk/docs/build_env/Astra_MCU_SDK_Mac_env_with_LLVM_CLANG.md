# Setup Guide for Development Environment on macOS for LLVM/Clang

## Table of Contents
- [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
- [Install CMake](#install-cmake)
- [Install Ninja Build System](#install-ninja-build-system)
- [Install Python](#install-python)
- [Install OpenOCD](#install-openocd)
- [Install the Arm GNU Toolchain](#install-the-arm-gnu-toolchain)
- [Install the LLVM Clang Compiler](#install-the-llvm-clang-compiler)

This guide applies to both Apple Silicon and Intel macOS; Homebrew prefix is detected automatically.

## Install Basic Tools and Dependencies

First, install [Homebrew](https://brew.sh/) if it's not already installed

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install essential tools:

```
brew update
brew install git wget make zip unzip python
```

## Install CMake

Download CMake 4.1.2 for macOS

```
curl -LO https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-macos-universal.tar.gz
```
Extract the archive

```
tar -xzf cmake-4.1.2-macos-universal.tar.gz
```

Move CMake to a system directory

```
sudo mv cmake-4.1.2-macos-universal /opt/cmake-4.1
```

Add CMake to PATH

```
echo 'export PATH="/opt/cmake-4.1/CMake.app/Contents/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```
Verify installation:

```
cmake --version
```

## Install Ninja Build System

Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds

Download Ninja v1.13.1 for macOS
```
curl -LO https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-mac.zip
```

Unzip the archive
```
unzip ninja-mac.zip
```

Create target directory and copy the binary
```
sudo mkdir -p /opt/ninja/
sudo cp ninja /opt/ninja/
```

Make the binary executable
```
sudo chmod a+x /opt/ninja/*
```

Add to PATH
```
echo 'export PATH=$PATH:/opt/ninja' >> ~/.zshrc
source ~/.zshrc
```

Verify installation:
```
ninja  --version
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
brew install pyenv
export PATH="$HOME/.pyenv/bin:$PATH"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"

pyenv install 3.13.7
pyenv local 3.13.7
python3 --version
```

Optional: make pyenv available in future shells (skip this if you prefer to enable pyenv manually per shell):

```bash
echo 'export PATH="$HOME/.pyenv/bin:$PATH"' >> ~/.zshrc
echo 'eval "$(pyenv init -)"' >> ~/.zshrc
echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.zshrc
```

If you chose the optional lines above, restart your shell or run `source ~/.zshrc` to apply them.

## Install OpenOCD

```bash
brew install openocd
openocd --version
```

## Install the Arm GNU Toolchain

Check your architecture:

```
uname -m
```

Download Arm GNU Toolchain 13.2.rel1 for macOS:

```
# Apple Silicon (arm64)
curl -Lo gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-arm64-arm-none-eabi.tar.xz

# Intel (x86_64)
curl -Lo gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz
```

Create target directory for installation
```
sudo mkdir -p /opt/gcc-arm-none-eabi
```

Extract the archive into the target directory
```
sudo tar -xf gcc-arm-none-eabi.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi
```

Environment Variables for Arm GNU Toolchain

```
echo 'export PATH=$PATH:/opt/gcc-arm-none-eabi/bin' >> ~/.zshrc
echo 'export GCC_TOOLCHAIN_ROOT=/opt/gcc-arm-none-eabi' >> ~/.zshrc
source ~/.zshrc
```

Note: LLVM builds require `GCC_TOOLCHAIN_ROOT` for the GCC sysroot and libstdc++.

## Install the LLVM Clang Compiler

Install LLVM/Clang 21.1.8
```
brew install llvm lld
```

Create target directory for installation
```
sudo mkdir -p /opt/llvm
```

Detect Homebrew prefix dynamically
```
BREW_PREFIX=$(brew --prefix)
```

Symlink Homebrew LLVM into /opt/llvm
```
ln -sfn ${BREW_PREFIX}/opt/llvm/bin /opt/llvm/bin
ln -sfn ${BREW_PREFIX}/opt/llvm/lib /opt/llvm/lib
ln -sfn ${BREW_PREFIX}/opt/llvm/include /opt/llvm/include
ln -sfn ${BREW_PREFIX}/opt/llvm/share /opt/llvm/share
```

Environment Variables for LLVM Clang

```
echo 'export PATH=$PATH:/opt/llvm/bin' >> ~/.zshrc
echo 'export LLVM_TOOLCHAIN_ROOT=/opt/llvm/bin' >> ~/.zshrc
source ~/.zshrc
```

> Note: LLVM compiler support is currently available only for SR110.
