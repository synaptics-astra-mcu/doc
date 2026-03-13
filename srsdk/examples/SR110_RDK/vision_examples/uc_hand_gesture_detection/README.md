# Hand Gesture Detection ML Application

## Description

The Hand Gesture Detection application is an ML-based computer vision solution designed for real-time gesture recognition with hand detection, single-hand tracking, and hand key-point estimation. The application detects hands in every frame, tracks one hand across frames, identifies one key point (at the center) for each hand location, and classifies the hand pose into one of the supported gesture classes. This use case supports HD resolution.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Hardware Requirements
- Sensor Adapter (included with the Astra Machina Micro kit)
- OV5647 Camera Sensor

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **hand_gesture_detection** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. In **Image Conversion**, open **Advanced Configurations** and edit `NVM_data.json`.
3. Set model flash offsets in `NVM_data.json`:
   - `image_offset_Model_A_offset`: `00607000`
   - `image_offset_Model_B_offset`: `00737000`
4. In **Image Flashing** (SWD/JTAG), flash the model binaries first:
   - `hand_gesture_detection_flash(1280x704).bin` at `0x607000`
   - `hand_gesture_detection_flash(320x320).bin` at `0x737000`
5. Flash the generated firmware image (`B0_flash_full_image_GD25LE128_67Mhz_secured.bin`).

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_hand_gesture_detection_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```

**Flash (CLI):**
1. Activate the SDK venv (required for image generation tools):
   ```bash
   # Linux/macOS
   source <sdk-root>/.venv/bin/activate
   # Windows PowerShell
   .\.venv\Scripts\Activate.ps1
   ```
2. Generate the flash image:
   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/spk_rc4_1_0_secure_otpk.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```
3. Flash model binaries first:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image examples/SR110_RDK/vision_examples/uc_hand_gesture_detection/models/hand_gesture_detection_flash(1280x704).bin \
     --flash-offset 0x607000

   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image examples/SR110_RDK/vision_examples/uc_hand_gesture_detection/models/hand_gesture_detection_flash(320x320).bin \
     --flash-offset 0x737000
   ```
4. Flash the firmware image:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```

---

## Running the Application using VS Code Extension

> **Windows note:** Ensure the USB drivers are installed for streaming. See the Zadig steps in  
> [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md#usb-cdc-image-streaming-windows).

1. In VS Code, open **Video Streamer** from the Synaptics sidebar.

   ![Video Streamer](assets/vs_video_streamer_toolbox.png)
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.

3. In the Video Streamer dropdown, select the **J13** COM port.
   - Plug in **J13** and press **RESET** on the board.
   - **Windows:** select the newly enumerated COM port.
   - **Linux/macOS:** select the lower-numbered COM port of the two newly enumerated ports.

4. Use the Video Streamer controls:

   a. Select **HAND_GESTURE_DETECTION** from the **UC ID** dropdown.  
   b. Set **RGB Demosaic** to **BayerGBRG**.  
   c. Click **Create Use Case**.  
   d. Click **Start Use Case** (a Python window opens and the video stream appears).

   ![video streamer controls](assets/hgd_video_controls.png)

5. For logs, use the **LOGGER** tab when needed.

6. Change **Visualization Modes** as needed:
   - Smart TV Gesture Control
   - 720p
   - 320x320
   - Text only

   ![Detection](assets/vs_hgd_detection.png)

7. **Autorun use cases:** If autorun is enabled, click **Connect Image Source** to open the video stream pop-up.

## Supported Hand Gestures

The following hand gestures are supported:

| Gesture | Description |
|---|---|
| One | One finger raised (index finger). |
| Two | Two fingers raised. |
| Three | Three fingers raised. |
| Four | Four fingers raised. |
| Five | Open palm with five fingers raised. |
| Fist | Closed fist with fingers folded inward. |
| Thumbs Up | Thumb raised upward with other fingers folded. |
| Thumbs Down | Thumb pointed downward with other fingers folded. |
| Pinch | Thumb and index finger brought close together (pinching pose). |

</br>

**Gesture one**

![one Gesture](assets/hgd_one.png)

**Gesture two**

![two Gesture](assets/hgd_two.png)

**Gesture Three**

![three Gesture](assets/hgd_three.png)

**Gesture four**

![Gesture four](assets/hgd_four.png)

**Gesture five**

![Gesture five](assets/hgd_five.png)

**Gesture fist**

![fist gesture](assets/hgd_fist.png)

**Gesture Thumbs Up**

![Thumbs up gesture](assets/hgd_thumbs_up.png)

**Gesture Thumbs Down**

![Thumbs down gesture](assets/hgd_thumbs_down.png)

**Gesture Pinch**

![Pinch Gesture](assets/hgd_pinch.png)
