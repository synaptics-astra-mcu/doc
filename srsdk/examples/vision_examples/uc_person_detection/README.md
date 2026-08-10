# Person Detection ML Application

## Description

The UC Person Detection application is designed to identify and locate persons within its field of view. It leverages object detection techniques to generate bounding boxes around detected individuals and assigns confidence scores to indicate the reliability of each detection. The output includes the precise location of each person in the image along with a confidence value, enabling accurate and efficient person recognition for various embedded vision applications. This example supports both WQVGA(480x270) and VGA(640x480) resolutions.

This usecase supports both **MIPI CSI** sensors (image-proc path) and **parallel/IIF** sensors (LP Sense data-flow path).

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `uc_person_detection.c`, and `uc_person_detection.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Project Configuration Selection

Before building, choose the project configuration (defconfig) that matches both your target board and the transfer mode you want to validate.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available defconfigs:**
- `sr110_rdk_cm55_parallel_camera_person_detection_defconfig` (PS5416 parallel / IIF)
- `sr110_rdk_cm55_person_detection_vga_img_proc_autorun_defconfig`
- `sr110_rdk_cm55_person_detection_vga_img_proc_defconfig`
- `sr110_rdk_cm55_person_detection_wqvga_img_proc_autorun_defconfig`
- `sr110_rdk_cm55_person_detection_wqvga_img_proc_defconfig`
- `sr110_rdk_cm55_person_detection_wqvga_lpsense_autorun_defconfig`
- `sr110_rdk_cm55_person_detection_wqvga_lpsense_defconfig`


For this app, the default defconfig is:
   - `sr110_rdk_cm55_person_detection_wqvga_img_proc_defconfig`

## Building and Flashing the Example using VS Code and CLI

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** → **Build Configurations**.
2. Select the **person_detection** project configuration in the **Project Configuration** dropdown.
3. If you need **VGA (640x480)**, click **Edit Configs** (Menuconfig) in the Build and Deploy view, then set  
   `COMPONENTS CONFIGURATION → Off Chip Components → Display Resolution` to **VGA**.
4. Optional configuration changes in Menuconfig:
   - **WQVGA in LP Sense**: `COMPONENTS CONFIGURATION → Drivers` → enable `MODULE_LP_SENSE_ENABLED`
   - **Static Image**: `COMPONENTS CONFIGURATION → Off Chip Components` → disable `MODULE_IMAGE_SENSOR_ENABLED`
5. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/vision_examples/uc_person_detection
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. If you need **VGA (640x480)**, open Kconfig and set  
   `COMPONENTS CONFIGURATION → Off Chip Components → Display Resolution` to **VGA**:
   ```bash
   make <app_defconfig> BOARD=SR110_RDK BUILD=SRSDK EDIT=1
   ```
3. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/vision_examples/uc_person_detection
   export SRSDK_DIR=<sdk-root>
   make build
   ```
4. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

**Build outputs (CLI):**
- Application binary: `<app-dir>/out/<target>/release/<target>.elf`
- App-local SDK package: `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. In **Image Conversion**, open **Advanced Configurations** and edit `NVM_data.json`.
3. Set the model flash offset in `NVM_data.json`:
   - `image_offset_Model_A_offset`: `00629000`
4. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.
5. **VGA use case:** flash the **model binary second**, after the **use case image**.  
   In **Image Flashing**, check **Model Binary** and set **Flash Offset** to `0x629000`, then flash the model file.  
   After that, flash the firmware image normally.

**Flash (CLI):**

1. Activate the SDK venv (required for image generation tools):
   ```bash
   # Linux/macOS
   source <sdk-root>/.venv/bin/activate
   # Windows PowerShell
   .\.venv\Scripts\Activate.ps1
   ```
2. Set the model flash offset in `tools/srsdk_image_generator/Input_Config/NVM_data.json`:
   - `image_offset_Model_A_offset`: `00629000`
3. Generate the flash image:
   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "<sdk-root>/tools/srsdk_image_generator/Inputs/spk_rc4_1_0_secure_otpk.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/Inputs/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/vision_examples/uc_person_detection/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```
4. Flash the firmware image:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```
5. **VGA use case:** flash the model binary second at offset `0x629000`:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image <path-to-model-bin> \
     --flash-offset 0x629000
   ```

---

## Running the Application using VS Code Extension

> **Windows note:** Ensure the USB drivers are installed for streaming. See the Zadig steps in  
> [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md#usb-cdc-image-streaming-windows).

1. In VS Code, open **Video Streamer** from the Synaptics sidebar.

   ![Video Streamer](assets/vs_video_streamer_toolbox.png)

2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower‑numbered J14 COM port first.
     - **Linux/macOS:** try the higher‑numbered J14 port first.
   - If you don’t see logs after a reset, switch to the other J14 port.
3. In the Video Streamer dropdown, select the **J13** COM port.
   - Plug in **J13** and press **RESET** on the board.
   - **Windows:** select the newly enumerated COM port.
   - **Linux/macOS:** select the lower‑numbered COM port of the two newly enumerated ports.
4. Use the Video Streamer controls:

   a. Select **PERSON_DETECTION** from the **UC ID** dropdown.  
   b. Set **RGB Demosaic** to **BayerRGGB**.  
   c. Click **Create Use Case**.  
   d. Click **Start Use Case** (a Python window opens and the video stream appears).

   ![Video Stream Window](assets/image_7.png)

5. **Autorun use cases:** If autorun is enabled, after step 4 click **Connect Image Source** to open the video stream pop-up.

## Adapting Pipeline for Custom Object Detection Models

This person detection pipeline can be adapted to work with custom object detection models. However, certain validation steps and potential modifications are required to ensure compatibility.

### Prerequisites for Model Compatibility

Before adapting this pipeline for another object detection model, you must verify the following:

#### 1. Model Format Requirements
- Your object detection model should be in `.tflite` format
- The model should produce similar output tensor structure (bounding boxes, confidence scores)

#### 2. Vela Compiler Compatibility Check

**Step 1: Analyze Original Model**
1. Load your `object_detection_model.tflite` file in [Netron](https://netron.app/)
2. Document the output tensors:
   - Tensor names
   - Tensor identifiers/indexes
   - Quantization parameters (scale and offset values)
   - Tensor dimensions

**Step 2: Compile with Vela**
1. Pass your model through the Vela compiler to generate `model_vela.bin` or `model_vela.tflite`
2. Analyze the Vela-compiled model in Netron using the same steps as above

**Step 3: Compare Outputs**
Compare the following between original and Vela-compiled models:
- **Output tensor indexes/identifiers**: Verify if they remain in the same order
- **Quantization parameters**: Check if scale and offset values are preserved
- **Tensor dimensions**: Ensure dimensions match your expected output format

### Pipeline Adaptation Process

#### Case 1: No Changes Required
If the Vela compilation preserves:
- ✅ Output tensor indexes in the same order
- ✅ Same quantization scale and offset values

**Result**: You can proceed with the existing pipeline without modifications.

#### Case 2: Modifications Required
If the Vela compilation changes:
- ❌ Output tensor index order
- ❌ Quantization parameters

**Required Actions**: Modify the pipeline code as described below.

### Code Modifications

If your model's output tensor indexes change after Vela compilation, you need to update the tensor parameter assignments in `uc_person_detection.c`:

#### Location: `detection_post_process` function

**Original Code:**
```c
g_box1_params = &g_all_tens_params[0];
g_box2_params = &g_all_tens_params[1];
g_cls_params  = &g_all_tens_params[2];
```

**Modified Code:**
Update the array indexes according to your Vela-compiled model's output tensor identifiers:
```c
// Example: If your model_vela output has different tensor order
g_box1_params = &g_all_tens_params[X];  // Replace X with actual index from Netron
g_box2_params = &g_all_tens_params[Y];  // Replace Y with actual index from Netron
g_cls_params  = &g_all_tens_params[Z];  // Replace Z with actual index from Netron
```
