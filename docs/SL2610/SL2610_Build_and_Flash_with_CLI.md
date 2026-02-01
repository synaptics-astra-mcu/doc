# SL2610 Build and Flash with CLI

This document provides concise, CLI-only steps to build and flash SL2610 applications.

## Prerequisites

- SL2610 RDK (Astra Machina Micro) connected and powered with both USB ports.
- CLI environment, toolchains, and Python tools installed. See [Setup and Install SDK using CLI](../Setup_and_Install_SDK_using_CLI.md).

## CLI Flow (Build + Flash)

1. Build the application:
    ```
    cd <sdk-root>/examples
    export SRSDK_DIR=<sdk-root>
    make cm52_sl2610_system_manager_defconfig BOARD=SL2610_RDK BUILD=SRSDK
    ```

2. Generate the flash image:

    ```
    #Build Bootloader for SL2610_RDK
    cd <sdk_root>
    make cm55_sr110_bootloader_defconfig BOARD=SL2610_RDK
    make build

    #Generate MCU Binaries
    cd examples
    make imagegen
    ```
    **Step 1: Copy MCU Binary into Yocto Build Tree**

    copy the generated binary to Yocto build repository: 

    ```
    build-sl261<X>/tmp/work/sl261<X>-poky-linux/synasdk-preboot/<GIT>/release/boot/mcu/cm52/image/chip/klamath/klamath_rdk/
    ```
    Example:

    ```
    build-sl2619/tmp/work/sl2619-poky-linux/synasdk-preboot/0.9.0+git/release/boot/mcu/cm52/image/chip/klamath/klamath_rdk/
    ```

    **Step 2: Build Astra Image via Yocto**

    Build an Image using Yocto - https://synaptics-astra.github.io/doc/v/latest/yocto.html

    ---

3. Flash the image:

    Flashing can be performed via Native CLI with the following commands using USB Boot Tool.
    **Tool path:** - `tools/usb_boot_python_tool/USB_BOOT_TOOL/usb_boot_tool.py`

    ```
    python usb_boot_tool.py --op run-sm --sm sysmgr.subimg
    python usb_boot_tool.py --op emmc --img-dir eMMCimg
    ```

    #### **Arguments Description**

    | Argument    | Description                                                            |
    | ----------- | ---------------------------------------------------------------------- |
    | `--op`      | Operation to perform. Supported values: `run-acore`, `run-sm`, `emmc`. |
    | `--sm`      | Path to the System Manager image (`sysmgr.subimg`).                    |
    | `--img-dir` | Path to the directory containing eMMC images (`eMMCimg`).              |

4. Reset the board:
   - Press the reset button.
   - The application should start and print logs on the serial console.

## Notes

- For more details refer: [SL2610 Platform Guide](./SL2610_platform_Guide.md)
