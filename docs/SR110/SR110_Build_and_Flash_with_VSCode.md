# SR110 Build and Flash with VS Code

This document provides concise, VS Code-only steps to build and flash SR110 applications.

## Prerequisites

- SR110 RDK (Astra Machina Micro) connected and powered with both USB ports.
- VS Code extension and tools installed. See [Setup and Install SDK using VSCode](../Setup_and_Install_SDK_using_VSCode.md).
- `examples/` imported and `SRSDK_DIR` set in VS Code.

## VS Code Flow (Build + Flash)

1. Import `examples/` and set `SRSDK_DIR`. See [Setup and Install SDK using VSCode](../Setup_and_Install_SDK_using_VSCode.md).
2. In the Synaptics extension locate the **IMPORTED EXAMPLE** side pannel and Open **Build and Deploy**.
   
   ![VS Code Side Panel](./assets/image_vs14.png)
   
   - Click the **Build Configurations** checkbox.
   - Set:
     - **Device**: SR110
     - **Build Type**: `cm55_fw`
     - **Build Mode**: Release or Debug
     - **Application**: demo_sample_app is a good place to start
     - Click the **Build (SDK + App)** checkbox
     - Click the **Clean (SDK + App)** checkbox
   ![Build and Deploy](./assets/vs_clean_build_sr110.png)

3. Click the **Image Conversion** checkbox. 
   - Click `Flash Image`
   - **Flash Type**: `GD25LE128`
   - **Image Security**: `Secured Image`
   - The **AXF/ELF File** will populate automatically after the build finishes

    ![Image Conversion](./assets/vs_image_conversion_option_sr110.png)
4. Click the **Image Flashing** checkbox.
   - **Interface**: `SWD/JTAG`
   - **Adapter**: `CMSIS-DAP`
   - Check the **Full Flash Erase** checkbox
   - **Binary File**: auto-populated after Image Conversion

    ![Image Flashing](./assets/image_vs32.png)
5. Click **Run** to to execute entire flow.

6. For instructions on how to run the example see the readme of that example. 

VS Code details: [VS Code Extension User Guide for SR110](./Astra_MCU_SDK_VSCode_Extension_User_Guide_SR110.md)
