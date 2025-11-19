# Astra MCU SDK Build System
## Contents
- [Requirements ](#requirements)
- [Build system](#project-build-system)
- [Build via native CLI](#building-astra-mcu-sdk-via-native-cli-tools)
    - [Build Astra MCU SDK package](#build-astra-mcu-sdk-package)
    - [Build example application](#build-example-application)
    - [Build TFlite and Bootloader](#build-tflite-and-bootloader)
      - [Load Default Configuration for Bootloader or TFlite](#load-default-bootloader-or-tflite-configuration)
      - [Menuconfig](#menuconfig)
      - [To start the bootloader or TFlite build](#to-start-the-bootloader-or-tflite-build)
    - [Clean the build](#clean-the-build)
    - [Help](#help)
    - [Generate Binary Images via native cli](#binary-generation)
        - [Generate Flash Binary](#generate-flash-binary)
        - [Generate Host Binary](#generate-host-binary)
        - [Arguments Description](#arguments-description)
- [Build via VS Code extension](#building-astra-mcu-sdk-via-vs-code-extension)
  - [Steps to build the SDK](#steps-to-build-the-sdk)
  - [Generate Binary Images via VS Code extension](#generate-binary-files-via-vs-code-extension)

The purpose of this document is to provide an overview of the build systems used in the Astra MCU SDK.

---

## Requirements

Please ensure that your system was set up according to one of the installations listed below
- Astra MCU SDK setup for Windows with ARM Compiler: [Setup_for_Astra_MCU_SDK_in_Windows_for_ARM_Compiler](./build_env/Astra_MCU_SDK_Windows_env_with_ARM_Compiler.md)
- Astra MCU SDK setup for Windows with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_Windows_for_GCC](./build_env/Astra_MCU_SDK_Windows_env_with_gcc.md)
- Astra MCU SDK setup for Linux with ARM Compiler : [Setup_for_Astra_MCU_SDK_in_Linux_for_ARM_Compiler](./build_env/Astra_MCU_SDK_Linux_env_with_ARM_Compiler.md)
- Astra MCU SDK setup for Linux with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_Linux_for_GCC](./build_env/Astra_MCU_SDK_Linux_env_with_gcc.md)
- Astra MCU SDK setup for macOS ARM64 with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_macOS_ARM64_for_GCC_Compiler](./build_env/Astra_MCU_SDK_Mac_Arm64_env_with_gcc.md)
- Astra MCU SDK setup for macOS x86_64 with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_macOS_x86_64_for_GCC](./build_env/Astra_MCU_SDK_Mac_x86_64_env_with_gcc.md)
- Astra MCU SDK VS Code Extension installation and setup: [Astra MCU SDK VS Code Extension User Guide](./Astra_MCU_SDK_VSCode_Extension_Userguide.rst)

---

## Project build system
- The configuration of parameters and the build process are handled by Kconfig configuration system and CMake build system along with Ninja generator respectively.
- Any terminal application can be used as the command-line build environment.
- The Astra MCU SDK is built and compiled using the CMake build flow, this flow requires manually calling several commands. To automate this flow a makefile has been created that automates all the steps. You can use this makefile to execute all the functionalities.
- The build system has two modes:
    - SDK build (BUILD=SRSDK), which compiles the SDK libraries and keeps the pre-built SDK libs in the **install** directory and then triggers the example build.
    - Application build (BUILD=EXAMPLE), which triggers the example build using the pre-built SDK.
> For more detailed description of the build workflow, please refer to [README.md](./examples/README.md)

---

## Building Astra MCU SDK via Native CLI Tools

### Build Astra MCU SDK package

To select desired default config for the target Board, the corresponding configuration is loaded into the .config file. This configuration determines which features, modules, and settings are included in the build.

Run the following to apply a default configuration:

```
make default_config BOARD=SR110_RDK
```

#### To start the build process

```makefile
make astrasdk
```

What this command does:

- Generate Configuration Header.

- The build system invokes Kconfiglib's genconfig script to generate the configuration header file (*config.h*) from the .config file.

- The generated *config.h* file is saved in the build directory.

- CMake is then configured using the project’s CMakeLists and the generated configuration.

- The Ninja build system is used to compile the sources and link them to produce the final executable.

#### Output

The Astra MCU SDK libs are compiled and the SDK package is installed in examples/install/`<BOARD>`


### Build Example Application

The example repository contains example applications for the Synaptics SDK (Astra MCU SDK), demonstrating various features and capabilities of the SR110 SoC.

To get an overview on the example build refer to [Build Example](./examples/README.md)

### Build TFlite and bootloader

#### Load Default Bootloader or TFlite Configuration

- The Astra MCU SDK offers several modules required for  applications, each demonstrating different features and use cases of the platform.
- To configure the build system for a specific project like bootloader or TFlite, you need to select an appropriate defconfig file.
- These files define the default configuration options for bootloader, TFlite and SDK default config are located in the *astra_mcu_sdk/configs/`<BOARD>`/* directory.

These are the available defconfig files in Astra MCU SDK

- cm55_sr110_bootloader_defconfig
- cm55_tflite_micro_defconfig

> *The actual list of available defconfig files may vary. Please refer to the astra_mcu_sdk/configs/`<BOARD>`/ directory for the latest options.*

Run the following command from astra_mcu_sdk directory to apply a default configuration:

```
make <bootloader/tflite>_defconfig BOARD=SR110_RDK
```

>💡Tip: Run *make list_defconfigs BOARD=SR110_RDK* to list the available defconfig files.

This applies the corresponding <TFlite/bootloader>_defconfig from astra_mcu_sdk/configs/`<BOARD>`/ and generates a .config file.

---

Example:

```
make cm55_sr110_bootloader_defconfig BOARD=SR110_RDK
```

Expected Output:

```
Loaded configuration 'astra_mcu_sdk/configs/SR110_RDK/configs cm55_sr110_bootloader_defconfig'
Configuration saved to '.config'
```

After loading the default configs, you can verify that the correct configurations are applied by inspecting the generated .config file:

```
cat .config
```

This step ensures the expected features and modules are enabled before building the project.

> Note: By default, these applications are configured to compile with GCC and are set to target the RDK board.

---

#### Menuconfig

A GUI based menuconfig interface is available to modify the project settings


```makefile
make menuconfig
```

This command will:

- Launch an interactive, terminal-based GUI interface.
- Display categorized configuration menus and options available for the project.
- Allow the user to enable, disable, or modify various settings.

    - In the *Astra MCU SDK Build Configuration* menu, you can select the Project, Build Type (Release / Debug / UnitTestDebug), Compiler (GCC / AC6), Board (SR110_RDK).

Changes made using menuconfig will update the .config file, which is used in subsequent build steps.

> *Make sure to save your changes before exiting the interface, otherwise modifications will be discarded.*

---

#### To start the bootloader or TFlite build

After selecting the desired application, the corresponding configuration is loaded into the .config file. This configuration determines which features, modules, and settings are included in the build.

```makefile
make build
```
>💡Tip: Running make without any arguments will automatically trigger the build

What this command does

- Generate Configuration Header.

- The build system invokes Kconfiglib's genconfig script to generate the configuration header file (*config.h*) from the .config file.

- The generated *config.h* file is saved in the build directory.

- CMake is then configured using the project’s CMakeLists and the generated configuration.

- The Ninja build system is used to compile the sources and link them to produce the final executable.

#### Output

The final output is an executable in AXF or ELF format.

Output Path:
out/<project_name>/<build_type>/<project_name>.elf or .axf

*Example:
out/sr110_bootloader/release/sr110_bootloader.elf*

### Clean the build

To clean the build

```makefile
make clean
```

### Help

The `make help` command displays a list of available build commands and their descriptions. It provides a quick reference for users to understand the available build targets, configuration options, and other utility commands within the build system.

Usage:

```
make help
```

Output:
```
To apply default settings for an application, run:
  make <name_of_application>
  example: make cm55_demo_sample_app

To build the project, just run:
  make
  or
  make build
Other useful targets:
  clean                 Remove build output directories
  help                  Show this help message
  list_defconfigs       List all available defconfig options
  menuconfig            Run interactive Kconfig menu
  save_defconfig        Save current minimal configuration as defconfig
  default_config        Apply the default configs to build the sdk package
  astrasdk              Build and Install the Astra MCU SDK package

```
---

### Binary Generation

After building the application, the generated .elf/.axf executable must be converted into a downloadable .bin image for flashing or host-based download.
Use the following commands to generate the required Flash and Host binary images from your build output.
The image conversion utility is located under *tools/srsdk_image_generator*.

#### Generate Flash Binary

```bash
python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>"
```

#### Generate Host Binary

```bash
python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "<path_to_spk_bin>" \
    -apbl "<path_to_apbl_axf>" \
    -m55_image "<path_to_m55_elf>"
```

---

#### **Arguments Description**

| Argument       | Description                                                      | Default Value                                                                           |
| -------------- | ---------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| `-B0`          | Target chip revision                                             | B0                                                                                      |
| `-flash_image` | Generate flash image (used for programming the external flash)   | —                                                                                       |
| `-host_image`  | Generate host image (used for USB/UART download)                 | —                                                                                       |
| `-sdk_secured` | Use secured SDK flow                                             | Enabled by default                                                                      |
| `-spk`         | Path to secure provisioning key `.bin` file (SPK)                | `tools/srsdk_image_generator/B0_input_examples/spk_rc3_0_secure_otpk_0605.bin`          |
| `-apbl`        | Path to AP Bootloader `.axf` file                                | `tools/srsdk_image_generator/B0_input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf` |
| `-m55_image`   | Path to the generated application `.elf` / `.axf` from SDK build | Generated by SDK build flow (e.g., `out/<project>/<type>/*.elf`)                        |
| `-model`       | Optional model name to be added to image metadata                | `""` (empty)                                                                            |
| `-flash_type`  | External flash part number                                       | `GD25LE128` *(Supported: GD25LE128, W25Q128, MX25U128)*                                 |
| `-flash_freq`  | SPI clock frequency for external flash (MHz)                     | `67` *(Supported: 34, 67, 100, 134)*                                                    |

---

> Note
The default *spk* and *apbl* files are located at:

```
tools/srsdk_image_generator/B0_input_examples/
```

> These defaults will work for typical SR110\_RDK secure flashing.
> Only override them if using a custom key or bootloader.

---

## Building Astra MCU SDK via VS Code extension


### Steps to build the SDK
   - Click on **Import SDK**, browse and select the **examples** directory. This will import the examples directory into the workspace.
   - Navigate to **IMPORTED REPOS** → **Build and Deploy** in the Astra MCU SDK VS Code Extension.
   - Select the **Build Configurations** checkbox, then select the necessary options.
   - Choose the application defconfig in the **Application** dropdown. This will apply the selected defconfig.
   - There are two build modes in the UI:
      a.	Build (SDK + App) – to install and build the application along with the SDK.
      b.	Build App – to build the example using the pre-built SDK.
   - Select the appropriate build and clean options from the checkboxes.
   - Then click **Run**. This will build the SDK generating the required `.elf` or `.axf` files for deployment using the installed package.

  > For detailed steps for non-native build, refer to the [Astra MCU SDK VS Code Extension Userguide](./Astra_MCU_SDK_VSCode_Extension_Userguide.rst).

---

### Generate Binary Files via VS Code Extension
  - Navigate to **IMPORTED REPOS** → **Build and Deploy** in Astra MCU SDK VS Code Extension.
  - Select the **Image Conversion** option, browse and select the required .axf or .elf file. If the usecase is built using the VS Code extension, the file path will be automatically populated.
  - Click **Run** to create the binary files.
> Refer to [Astra MCU SDK VS Code Extension User Guide](./Astra_MCU_SDK_VSCode_Extension_Userguide.rst) for more detailed instructions.

---