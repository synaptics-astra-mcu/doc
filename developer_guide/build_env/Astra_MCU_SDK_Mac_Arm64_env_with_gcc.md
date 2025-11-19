# Setup Guide for Development Environment on MAC Arm64 for GCC

Content:
  - [Install Basic Tools and Dependencies](#mac-arm64-install-basic-tools-and-dependencies)
  - [Install CMake](#mac-arm64-install-cmake)
  - [Install Ninja Build System](#mac-arm64-install-ninja-build-system)
  - [Install the ARM GNU GCC Compiler](#mac-arm64-install-the-arm-gnu-gcc-compiler)

(mac-arm64-install-basic-tools-and-dependencies)=
## Install Basic Tools and Dependencies

First, install [Homebrew](https://brew.sh/) if it's not already installed

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install essential tools:

```
brew update
brew install git wget make python3
```

(mac-arm64-install-cmake)=
## Install CMake

Download CMake 3.26.0 Binary for macOS

```
curl -LO https://github.com/Kitware/CMake/releases/download/v3.26.0/cmake-3.26.0-macos-universal.tar.gz
```
Extract the Archive

```
tar -xzf cmake-3.26.0-macos-universal.tar.gz
```

Move CMake to a System Directory

```
sudo mv cmake-3.26.0-macos-universal /opt/cmake-3.26
```

Add CMake to PATH

```
echo 'export PATH="/opt/cmake-3.26/CMake.app/Contents/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```
verify installation

```
cmake --version
```

(mac-arm64-install-ninja-build-system)=
## Install Ninja Build System

Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds

Download the latest macOS Ninja binary (v1.11.1)
```
curl -LO https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-mac.zip
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

(mac-arm64-install-the-arm-gnu-gcc-compiler)=
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
