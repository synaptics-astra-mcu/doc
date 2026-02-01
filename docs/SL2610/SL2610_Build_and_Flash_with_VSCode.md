# SL2610 Build and Flash with VS Code

This document provides concise, VS Code-only steps to build and flash SL2610 applications.

## Prerequisites

- SL2610 RDK (Astra Machina Micro) connected and powered with both USB ports.
- VS Code extension and tools installed. See [Setup and Install SDK using VSCode](../Setup_and_Install_SDK_using_VSCode.md).
- `examples/` imported and `SRSDK_DIR` set in VS Code.

## VS Code Flow (Build + Flash)

### Build Configurations SL2610
<a id="build-configurations-sl2610"></a>

**Purpose:** Build SL2610 firmware (cm52) and provide SL-specific image generation/flashing controls.

**Steps:**
1. Set the workspace `SRSDK_DIR` via the Import SDK view so the Build UI can detect the SDK.
2. Open the Build & Deploy webview and set `Project` → `SL2610`.
3. Select the desired `Board` (RDK or PEK) and confirm `Project` shows `cm52_fw`.
4. Choose `Release` and an available `Toolchain` if needed.
5. Enable the Build Configurations toggle and click `Run` to build.

![SL2610 Build UI placeholder](./assets/vs_build_sl2610.png)

### Image Generation SL2610

**Steps:**

1.  Click on the "Image Generation" panel.

2.  In the file path, already the Release build file path will be
    pre-populated (if already built for Release option). Also, the user
    can select a custom MCU executable file for converting using the "Browse"
    button.

3.  Click Run to convert the MCU executable to sub-image.

**Result**

The resultant sub-image will be generated in `out/sl2610_cm52_fw/release/sysmgr.subimg.gz`.

>Note: Image Generation feature for SL2610 is available only on Linux platforms.

### Image Flashing for SL2610

<a id="image-flashing-sl2610"></a>

Choose the target as `M52 Image` or `Full Image` in the `Flash Target` dropdown.
>The **M52 Image** option corresponds to System Manager Sub-Image.
>The **Full Image** option correspomds to eMMC Imaege.

**M52 Image flashing requirements:**
- Provide the sysmgr subimg file path in the Image flashing panel.
- To select a custom sysmgr subimg use the `Browse` option to select the file.
>Note: If the workflow is in `unified mode` the generated sysmgr subimg after image generation will get populated to image flashing panel automatically.

![alt text](./assets/vs_image_flash_sl2610.png)

**Full Image flashing requirements:**
- Provide the eMMC folder path that contains `emmc_part_list` and `emmc_image_list`. The extension uses these files to determine partitions and image order for flashing.
- Optionally enable "Use Custom SM Image" to override the default SM image (use with caution).

![alt text](./assets/vs_image_flash_emmc_sl2610.png)

VS Code details: [VS Code Extension User Guide for SL2610](./Astra_MCU_SDK_VSCode_Extension_User_Guide_SL2610.md)
