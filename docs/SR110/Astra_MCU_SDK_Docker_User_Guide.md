# Astra MCU SDK Docker User Guide

## Contents

- [Docker Setup, Build & Image generation Instructions: Windows](#docker-setup-build--image-generation-instructions-windows)
    - [Docker Setup](#docker-setup)
    - [Build Docker Image and Run Container](#build-docker-image-and-run-container)
    - [Build the Project Inside Docker Container using GCC compiler](#build-the-project-inside-docker-container-using-gcc-compiler)
    - [Build the Project Inside Docker Container using AC6 compiler](#build-the-project-inside-docker-container-using-ac6-compiler)
    - [Image generation steps](#image-generation-steps)
    - [Copy the output file to a native directory](#copy-the-output-file-to-a-native-directory)
- [Docker Setup, Build & Image generation Instructions: Linux](#docker-setup-build--image-generation-instructions-linux)
    - [Docker Setup](#docker-setup-1)
    - [Build Docker Image and Run Container](#build-docker-image-and-run-container-1)
    - [Build the Project Inside Docker Container using GCC compiler](#build-the-project-inside-docker-container-using-gcc-compiler-1)
    - [Build the Project Inside Docker Container using AC6 compiler](#build-the-project-inside-docker-container-using-ac6-compiler-1)
    - [Image generation steps](#image-generation-steps-1)
    - [Copy the output file to a native directory](#copy-the-output-file-to-a-native-directory-1)
- [Docker Setup, Build & Image generation Instructions: macOS](#docker-setup-build--image-generation-instructions-macos)
    - [Docker Setup](#docker-setup-2)
    - [Build Docker Image and Run Container](#build-docker-image-and-run-container-2)
    - [Build the Project Inside Docker Container using GCC compiler](#build-the-project-inside-docker-container-using-gcc-compiler-2)
    - [Image generation steps](#image-generation-steps-2)
    - [Copy the output file to a native directory](#copy-the-output-file-to-a-native-directory-2)

## Docker Setup, Build & Image generation Instructions: Windows

### Docker Setup

#### WSL Setup

- To check if WSL is installed, open PowerShell and run
    ```
    wsl --status
    ```

- To install WSL (default Ubuntu), Open PowerShell as Administrator and run
    ```
    wsl --install
    ```

- List Available Linux Distributions
    ```
    wsl --list –online
    ```

- Install a Specific Distribution
    ```
    wsl --install -d <DistroName>
    ```

-   Set Ubuntu as Default Distribution
    ```
    wsl --set-default Ubuntu
    ```

- To start WSL
    ```
    wsl or wsl -d <DistroName>
    ```

#### Install Docker Engine inside WSL (Ubuntu Terminal)

- Install required packages
    ```
    sudo apt-get install ca-certificates curl   
    sudo install -m 0755 -d /etc/apt/keyrings   
    sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc   
    sudo chmod a+r /etc/apt/keyrings/docker.asc
    ```

- Add Docker repository
    ```
    echo "deb [arch=$(dpkg --print-architecture) signed-
    by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/$(. /etc/os-
    release; echo "$ID") $(lsb_release -cs) stable" | sudo tee 
    /etc/apt/sources.list.d/docker.list > /dev/null
    ```

- Update and Install Docker
    ```
    sudo apt-get update 
    #Make sure that correct GPG key is present
    ```

- If GPG key is missing or incorrect

    - Remove the old Docker GPG key (cleanup) 
        ```
        sudo rm /etc/apt/keyrings/docker.gpg 2>/dev/null
        ```

    - Add Docker’s Official GPG Key
        ```
        sudo install -m 0755 -d /etc/apt/keyrings
        curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --
        dearmor -o /etc/apt/keyrings/docker.gpg
        sudo chmod a+r /etc/apt/keyrings/docker.gpg
        ```
    - Add the Docker Repository Properly
        ```
        echo "deb [arch=$(dpkg --print-architecture) signed-
        by=/etc/apt/keyrings/docker.gpg]
        https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" |
        sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
        ```
    - Update Package List Again
        ```
        sudo apt-get update
        ```

- Install Docker
    ```
    sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
    ```

- To verify Docker installation
    ```
    sudo docker run hello-world
    ```

- Run Docker without sudo
    ```
    sudo groupadd docker
    sudo usermod -aG docker $USER
    ```
    Then restart WSL session and test again, 
    ```
    docker run hello-world
    ```

- Verify docker installation
    ```  
    docker --version
    docker info
    ```

#### Build Docker Image and Run Container

- To build the Docker image, navigate to the base directory of the SDK project.

- Build the Docker Image
    ```
    docker build -t <container_name>
    ```
    > Note: Replace <container_name> with preferred docker image name

- Check Docker Status
    ```
    docker images
    docker ps
    ```

- Execute the following command to run the container in an interactive session and mount SDK root directory and the examples folder
    ```
    docker run --rm -it -v “<path_to_sdk_rootfolder>”:/workspace/srsdk -v "<path_to_examples_folder>":/workspace/examples <container_name>
    ```

- Example
    ```
    docker run --rm -it -v "/mnt/c/Users/priya/Downloads/srsdk":/workspace/srsdk -v "/mnt/c/Users/priya/Downloads/examples":/workspace/examples devcont:latest
    ```
    > Note: Use Linux path(/mnt/c/User/path) to mount your SDK, not Windows path(C:/Users/path).

#### Build the Project Inside Docker Container using GCC compiler

- Inside the container (Interactive Mode), navigate to examples directory from your base SDK folder: `cd examples`

- To set environment variables, Run
    ```
    export SRSDK_DIR=/workspace/srsdk
    export GCC_TOOLCHAIN_13_2_1=/opt/gcc-arm-none-eabi/bin
    source ~/.bashrc
    ```

- To install and build the application along with sdk
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK
    ```

- Example:
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
    ```

- To build using menuconfig interface
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK EDIT=1
    ```

- Example:
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
    ```

- For building the already installed package (recurrent build):
    ```
    make build
    ```

- After successful build, out files will be written to `/examples/out directory`

- For cleaning the build directory along with installed SDK package:
    ```
    make clean all
    ```

- For cleaning the build directory
    ```
    make clean
    ```

#### Build the Project Inside Docker Container using AC6 compiler

##### Download AC6 Compiler

- Go to the Arm Compiler downloads index

- Download version 6.19 for your target platform
    - **Windows:** `ARMCompiler6.19_standalone_win-x86_64.zip`
    - **Linux:** `ARMCompiler6.19_standalone_linux-x86_64.tar.gz or aarch64`
    - **Mac:** `ARMCompiler6.19_standalone_mac-x86_64.tar.gz`

- Place the downloaded file inside your SDK folder (e.g.,C:\Users\Username\srsdk)

##### Build the Docker Image, run container and mount sdk

To build the Docker image, navigate to the base directory of the SDK project.

- Build the Docker Image
    ```
    docker build -t <container_name>
    ```
    > Note: Replace <container_name> with preferred docker image name.

- Check Docker Status
    ```
    docker images
    docker ps
    ```

- Execute the following command to run the container in an interactive session and mount SDK root directory and the examples folder
    ```
    docker run --rm -it -v “<path_to_sdk_rootfolder>”:/workspace/srsdk -v "<path_to_examples_folder>":/workspace/examples <container_name>
    ```

- Example:
    ```
    docker run --rm -it -v "/mnt/c/Users/priya/Downloads/srsdk":/workspace/srsdk -v "/mnt/c/Users/priya/Downloads/examples":/workspace/examples devcont:latest
    ```
    > Note: Use Linux path(/mnt/c/User/path) to mount your SDK, not Windows path(C:/Users/path)

##### License Activation

- While running a container interactively
    ```
    export ARMLM_ONDEMAND_ACTIVATION='HWSKT-EAC0@http://sjc1uvp-elict08.synaptics.com:7070'
    ```

##### Set AC6 as compiler and build SDK

- Navigate to base SDK folder. Open build configuration Menu. Inside docker container interactive shell
    ```
    make menuconfig
    ```

- Navigate to Compiler Settings, Press Save & Exit
    ```
    SRSDK BUILD CONFIGURATION →  Compiler  →  Select “AC6”
    ```

- Inside the container (Interactive Mode), navigate to examples directory from your base SDK folder
    ```
    cd examples
    ```

- To set environment variables, run
    ```
    export SRSDK_DIR=/workspace/srsdk 
    export AC6_TOOLCHAIN_6_19_0=<path to AC6 compiler bin>
    source ~/.bashrc
    ```

- To install and build the application along with SDK
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
    ```

- To build using menuconfig interface
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK EDIT=1
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
    ```

- For building the already installed package (recurrent build)
    ```
    make build
    ```

- After successful build, out files will be written to `/examples/out directory`

- For cleaning the build directory along with installed SDK package
    ```
    make clean all
    ```

- For cleaning the build directory
    ```
    make clean
    ```

### Image generation steps

- In SRSDK Folder, inside container (interactive mode), go to the image generator tool directory
    ```
    cd /workspace/srsdk/tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator
    ```

- In Release package, inside container (interactive mode), go to the image generator tool directory
    ```
    cd /workspace/srsdk/tools/srsdk_image_generator
    ```

- **Generate Flash Binary**
    ```
    python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \  
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \  
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \  
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -model "" \
    -flash_type "GD25LE128" \
    -flash_freq "67” \
    -single_slot
    ```

- **Generate Host Binary**
    ```
    python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \  
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -single_slot
    ```

- **Generate flash and host binary**
    ```
    python srsdk_image_generator.py \
    -B0 \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -model "" \
    -flash_type "GD25LE128" \
    -flash_freq "67" \
    -single_slot
    ```

### Copy the output file to a native directory

> Note: Exit the Docker container’s interactive shell before copying the required files to the host system.

- The generated build file (.elf/.axf) will be located inside the `/workspace/examples/out/sr110_cm55_fw/release directory within the SDK`

- Copy it to a native (host) directory 
    ```
    cp /path/to/sdk/examples/out/sr110_cm55_fw/release /path/to/native/destination/
    ```

- In SRSDK Folder, the generated binary image (.bin) will be located inside tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator/Output/B0_<image_type>
    ```
    cp /path/to/sdk/tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator/Output/B0_<image_type>/ /path/to/destination
    ```

- In Release package, the generated binary image (.bin) will be located inside `srsdk/tools/srsdk_image_generator/Output/B0_<image_type>`
    ```
    cp /path/to/sdk/tools/srsdk_image_generator/Output/B0_<image_type>/ /path/to/destination
    ```

## Docker Setup, Build & Image generation Instructions: Linux

### Docker Setup

- Identify the host system Architecture
    ```
    uname -m
    ```
    - If output is x86_64 → Intel/AMD 64-bit (supported)
    - If output is aarch64 → ARM64 (supported)
    - Otherwise → Architecture not supported
    </br>

- Remove any old Docker versions
    ```
    sudo apt-get remove -y docker docker-engine docker.io containerd runc
    ```

- Update Package Index and install required dependencies
    ```
    sudo apt-get update -y
    sudo apt-get install -y ca-certificates curl gnupg lsb-release
    ```
- Add Docker’s Official GPG Key
    ```
    sudo install -m 0755 -d /etc/apt/keyrings && curl -fsSL
    https://download.docker.com/linux/$(. /etc/os-release; echo "$ID")/gpg | sudo
    gpg --dearmor -o /etc/apt/keyrings/docker.gpg && sudo chmod a+r
    /etc/apt/keyrings/docker.gpg
    ```
- Add Docker repository:
    ```
    echo "deb [arch=$(dpkg --print-architecture) signed-
    by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/$(. /etc/os-
    release; echo "$ID") $(lsb_release -cs) stable" | sudo tee
    /etc/apt/sources.list.d/docker.list > /dev/null
    ```

- Install Docker Engine, CLI, and Compose Plugin
    ```
    sudo apt-get update -y
    apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
    ```

- To use docker without sudo
    ```
    sudo usermod -aG docker "$USER"
    ```

- After this, log out and log back in, or use
    ```
    newgrp docker
    ```

- Enable and start docker service
    ```
    sudo systemctl enable docker
    sudo systemctl start docker 
    ```

- Verify Docker installation
    ```
    docker --version
    docker info
    ```

### Build Docker Image and Run Container

- To build the Docker image, navigate to the base directory of the SDK project

- Build the Docker Image
    ```
    docker build -t <container_name>
    ```
    > Note: Replace <container_name> with preferred docker image name

- Check Docker Status
    ```
    docker images
    docker ps
    ```

- Execute the following command to run the container in an interactive session and mount SDK root directory and the examples folder
    ```
    docker run --rm -it -v “<path_to_sdk_rootfolder>”:/workspace/srsdk -v "<path_to_examples_folder>":/workspace/examples <container_name>
    ```
- Example
    ```
    docker run --rm -it -v "/mnt/c/Users/priya/Downloads/srsdk":/workspace/srsdk -v "/mnt/c/Users/priya/Downloads/examples":/workspace/examples devcont:latest
    ```
    > Note: Use Linux path(/mnt/c/User/path) to mount your SDK, not Windows path(C:/Users/path)

### Build the Project Inside Docker Container using GCC compiler

- Inside the container (Interactive Mode), navigate to examples directory from your base SDK folder
    ```
    cd examples
    ```

- To set environment variables, Run
    ```
    export SRSDK_DIR=/workspace/srsdk
    export GCC_TOOLCHAIN_13_2_1=/opt/gcc-arm-none-eabi/bin
    source ~/.bashrc
    ```

- To install and build the application along with sdk
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
    ```

- To build using menuconfig interface
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK EDIT=1
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
    ```

- For building the already installed package (recurrent build)
    ```
    make build
    ```

- After successful build, out files will be written to `/examples/out directory`

- cleaning the build directory along with installed SDK package
    ```
    make clean all
    ```

- For cleaning the build directory
    ```
    make clean
    ```

### Build the Project Inside Docker Container using AC6 compiler

#### Download AC6 Compiler

- Go to the Arm Compiler downloads index

- Download version 6.19 for your target platform
        - **Windows:** ARMCompiler6.19_standalone_win-x86_64.zip
        - **Linux:** ARMCompiler6.19_standalone_linux-x86_64.tar.gz or aarch64
        - **Mac:** ARMCompiler6.19_standalone_mac-x86_64.tar.gz

- Place the downloaded file inside your SDK folder (e.g.,C:\Users\Username\srsdk)

#### Build the Docker Image, run container and mount sdk:

- To build the Docker image, navigate to the base directory of the SDK project.

- Build the Docker Image
    ```
    docker build -t <container_name>
    ```
    > Note: Replace <container_name> with preferred docker image name

- Check Docker Status
    ```
    docker images
    docker ps
    ``` 

- Execute the following command to run the container in an interactive session and mount SDK root directory and the examples folder
    ```
    docker run --rm -it -v “<path_to_sdk_rootfolder>”:/workspace/srsdk -v "<path_to_examples_folder>":/workspace/examples <container_name>
    ```

- Example
    ```
    docker run --rm -it -v "/mnt/c/Users/priya/Downloads/srsdk":/workspace/srsdk -v "/mnt/c/Users/priya/Downloads/examples":/workspace/examples devcont:latest
    ```
    > Note: Use Linux path(/mnt/c/User/path) to mount your SDK, not Windows path(C:/Users/path)

#### License Activation

- While running a container interactively set the license query
    ```
    export ARMLM_ONDEMAND_ACTIVATION='HWSKT-EAC0@http://sjc1uvp-elict08.synaptics.com:7070'
    ```

#### Set AC6 as compiler and build SDK

- Navigate to base SDK folder. Open build configuration Menu. Inside docker container interactive shell
    ```
    make menuconfig
    ```

- Navigate to Compiler Settings, Press Save & Exit
    ```
    SRSDK BUILD CONFIGURATION →  Compiler  →  Select “AC6"
    ```

- Inside the container (Interactive Mode), navigate to examples directory from your base SDK folder
    ```
    cd examples 
    ```

- To set environment variables, run
    ```
    export SRSDK_DIR=/workspace/srsdk
    export AC6_TOOLCHAIN_6_19_0=<path to AC6 compiler bin>
    source ~/.bashrc
    ```

- To install and build the application along with SDK
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
    ```

- To build using menuconfig interface
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK EDIT=1
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
    ```

- For building the already installed package (recurrent build)
    ```
    make build
    ```

- After successful build, out files will be written to `/examples/out directory`

- For cleaning the build directory along with installed SDK package
    ```
    make clean all
    ```

- For cleaning the build directory
    ```
    make clean
    ```

### Image generation steps

- In SRSDK Folder, inside container (interactive mode), go to the image generator tool directory
    ```
    cd /workspace/srsdk/tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator
    ```

- In Release package, inside container (interactive mode), go to the image generator tool directory
    ```
    cd /workspace/srsdk/tools/srsdk_image_generator
    ```

- Generate Flash Binary
    ```
    python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \  
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \  
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>" \ 
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -model "" \
    -flash_type "GD25LE128" \
    -flash_freq "67” \
    -single_slot
    ```

- Generate Host Binary
    ```
    python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -single_slot
    ```

- Generate flash and host binary
    ```
    python srsdk_image_generator.py \
    -B0 \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -model "" \
    -flash_type "GD25LE128" \
    -flash_freq "67" \
    -single_slot
    ```

### Copy the output file to a native directory

> Note: Exit the Docker container’s interactive shell before copying the required files to the host system.

- The generated build file (.elf/.axf) will be located inside the `/workspace/examples/out/sr110_cm55_fw/release` directory within the SDK

- Copy it to a native (host) directory
    ```
    cp /path/to/sdk/examples/out/sr110_cm55_fw/release /path/to/native/destination/
    ```

- In SRSDK Folder, the generated binary image (.bin) will be located inside `tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator/Output/B0_<image_type>`
    ```
    cp /path/to/sdk/tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator/Output/B0_<image_type>/ /path/to/destination
    ```

- In Release package, the generated binary image (.bin) will be located inside `srsdk/tools/srsdk_image_generator/ Output/B0_<image_type>`
    ```
    cp /path/to/sdk/tools/srsdk_image_generator/Output/B0_<image_type>/ /path/to/destination
    ```

## Docker Setup, Build & Image generation Instructions: macOS

### Docker Setup

#### Install Homebrew (If not already installed)

- Install Homebrew
    ```
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)”
    ```

- Add Homebrew to your shell
    ```
    echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
    eval "$(/opt/homebrew/bin/brew shellenv)"
    ```

- Verify Installation
    ```
    brew --version
    ```

#### Install Colima and Docker CLI 

- Install colima and docker CLI
    ```
    brew install colima docker docker-compose
    ```

- Verify Installation
    ```
    colima --version
    docker --version
    docker-compose --version
    ```

#### Prepare colima directories and start colima with docker runtime

- Configure colima
    ```
    mkdir -p ~/.colima/tmp
    mkdir -p ~/.colima/config
    chmod -R 777 ~/.colima
    export TMPDIR="$HOME/.colima/tmp"
    ```
- Start colima
    ```
    colima start --cpu 4 --memory 8 --disk 60 --runtime docker
    ```

- To display the current status of colima virtual machine
    ```
    colima status
    ```

- To connect to the colima virtual machine via ssh to build docker image
    ```
    colima ssh
    ```

#### Build Docker Image and Run Container

- To build the Docker image, navigate to the base directory of the SDK project.

- Build the Docker Image
    ```
    docker build -t <container_name>
    ```
    > Note: Replace <container_name> with preferred docker image name

- Check Docker Status
    ```
    docker images
    docker ps
    ```

- Execute the following command to run the container in an interactive session and mount SDK root directory and the examples folder
    ```
    docker run --rm -it -v “<path_to_sdk_rootfolder>”:/workspace/srsdk -v "<path_to_examples_folder>":/workspace/examples <container_name>
    ```

- Example
    ```
    docker run --rm -it -v "/Users/priya/Downloads/srsdk":/workspace/srsdk -v "/Users/priya/Downloads/examples":/workspace/examples devcont:latest
    ```
    > Note: Use Standard Unix-style path (/Users/path) to mount your SDK, not the other ones.

#### Build the Project Inside Docker Container using GCC compiler

- Inside the container (Interactive Mode), navigate to examples directory from your base SDK folder
    ```
    cd examples
    ```

- To set environment variables, Run
    ```
    export SRSDK_DIR=/workspace/srsdk
    export GCC_TOOLCHAIN_13_2_1=/opt/gcc-arm-none-eabi/bin
    source ~/.bashrc
    ```
- To install and build the application along with sdk
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
    ```

- To build using menuconfig interface
    ```
    make <defconfigname> BOARD=<board> BUILD=SRSDK EDIT=1
    ```

- Example
    ```
    make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
    ```

- For building the already installed package (recurrent build)
    ```
    make build
    ```

- After successful build, out files will be written to `/examples/out directory`

- For cleaning the build directory along with installed SDK package
    ```
    make clean all
    ```

- For cleaning the build directory
    ```
    make clean
    ```


### Image generation steps

- In SRSDK Folder, inside container (interactive mode), go to the image generator tool directory
    ```
    cd /workspace/srsdk/tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator
    ```

- In Release package, inside container (interactive mode), go to the image generator tool directory
    ```
    cd /workspace/srsdk/tools/srsdk_image_generator
    ```

- Generate Flash Binary
    ```
    python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \  
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \  
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>" \ 
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -model "" \
    -flash_type "GD25LE128" \
    -flash_freq "67” \
    -single_slot
    ```

- Generate Host Binary
    ```
    python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -single_slot
    ```

- Generate flash and host binary
    ```
    python srsdk_image_generator.py \
    -B0 \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>" \
    -single_slot
    ```

- Example
    ```
    python srsdk_image_generator.py \
    -B0 \
    -sdk_secured \
    -spk "B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
    -apbl "B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
    -m55_image "/workspace/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
    -model "" \
    -flash_type "GD25LE128" \
    -flash_freq "67" \
    -single_slot
    ```

### Copy the output file to a native directory

> Note: Exit the Docker container’s interactive shell before copying the required files to the host system.

- The generated build file (.elf/.axf) will be located inside the `/workspace/examples/out/sr110_cm55_fw/release` directory within the SDK

- Copy it to a native (host) directory
    ```
    cp /path/to/sdk/examples/out/sr110_cm55_fw/release /path/to/native/destination/
    ```

- In SRSDK Folder, the generated binary image (.bin) will be located inside `tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator/Output/B0_<image_type>`
    ```
    cp /path/to/sdk/tools/syna_toolkit/syna_gui/syna_main_gui_app/srsdk_image_generator/Output/B0_<image_type>/ /path/to/destination
    ```

- In Release package, the generated binary image (.bin) will be located inside `srsdk/tools/srsdk_image_generator/ Output/B0_<image_type>`
    ```
    cp /path/to/sdk/tools/srsdk_image_generator/Output/B0_<image_type>/ /path/to/destination
    ```