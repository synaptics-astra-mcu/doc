# Setup Guide for Development Environment on MAC Arm64 for GCC

Content:

- [Setup Guide for Development Environment on Mac Arm64 for GCC](#setup-guide-for-development-environment-on-mac-arm64-for-gcc)
  - [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
  - [Install CMake](#install-cmake)
  - [Install Ninja Build System](#install-ninja-build-system)
  - [Install Python](#install-python)
  - [Install the ARM GNU GCC Compiler](#install-the-arm-gnu-gcc-compiler)

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

Download CMake 4.1.2 Binary for macOS

```
curl -LO https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-macos-universal.tar.gz
```
Extract the Archive

```
tar -xzf cmake-4.1.2-macos-universal.tar.gz
```

Move CMake to a System Directory

```
sudo mv cmake-4.1.2-macos-universal /opt/cmake-4.1
```

Add CMake to PATH

```
echo 'export PATH="/opt/cmake-4.1/CMake.app/Contents/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```
Verify installation

```
cmake --version
```

## Install Ninja Build System

Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds

Download the latest macOS Ninja binary (v1.13.1)
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

Verify Installation:
```
ninja  --version
```

## Install Python

Python is required to run configuration tools (menuconfig, kconfig) and to execute scripts used during SDK build and setup.

Download the latest Python version (3.13.7)
```
brew install pyenv
pyenv install 3.13.7
pyenv global 3.13.7
```
Verify Installation:
```
python3 --version
```

## Install the ARM GNU GCC Compiler

Download the macOS (Apple Silicon) version of the ARM GNU Toolchain
```
curl -Lo gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-arm64-arm-none-eabi.tar.xz
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
echo 'export GCC_TOOLCHAIN_13_2_1=/opt/gcc-arm-none-eabi/bin' >> ~/.zshrc
source ~/.zshrc
```
