# Person Segmentation ML Application

## Description

The UC Segmentation application is designed to detect and segment persons within the camera's field of view. It generates pixel-wise masks that accurately outline each detected individual, along with corresponding bounding boxes and confidence scores. The output includes segmented regions that represent the exact shape of each person in the image, providing both spatial and confidence-level insights for each detection. This example supports both WQVGA(480x270) and VGA(640x480) resolutions.

## Build Instructions

### Prerequisites
- [GCC/AC6 build environment setup](../developer_guide/build_env.rst)
- [Astra SRSDK VS Code Extension installed and configured](../developer_guide/SRSDK_VSCode_Extension_Userguide.rst)
- [SynaToolkit installed and configured](../subject/toolkit/toolkit.rst)

### Configuration and Build Steps

1. **Select Default Configuration and build sdk + example**
   This will apply the defconfig, then build and install the SDK package, generating the required `.elf` or `.axf` files for deployment using the installed package.
   ```bash
   make cm55_person_segmentation_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```
   This configuration uses WQVGA resolution by default.

2. **Edit default configs and build sdk + example**

   >💡Tip: Run `make cm55_person_segmentation_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1` to modify the configuration via a GUI and proceed with build.

   | Configuration | Menu Navigation | Action |
   |---------------|-----------------|---------|
   | **VGA Resolution** | `COMPONENTS CONFIGURATION → Off Chip Components → Display Resolution` | Change to `VGA(640x480)` |
   | **WQVGA in LP Sense** | `COMPONENTS CONFIGURATION → Drivers` | Enable `MODULE_LP_SENSE_ENABLED` |
   | **Static Image** | `COMPONENTS CONFIGURATION → Off Chip Components` | Disable `MODULE_IMAGE_SENSOR_ENABLED` |

3. **Rebuild the Application using pre-built package**
   The build process will produce the necessary .elf or .axf files for deployment with the installed package.
   ```bash
   make cm55_person_segmentation_defconfig BOARD=SR110_RDK or make
   ```
   **Note:** We need to have the pre-built SRSDK package before triggering the example alone build.
## Deployment and Execution

### Setup and Flashing

1. **Open Image Flashing Window**
   Navigate to the Image Flashing Window on Astra SRSDK VS Code Extension.
   For detailed steps on generating and flashing images, refer to the [Astra SRSDK VSCode Extension User Guide](../developer_guide/SRSDK_VSCode_Extension_Userguide.rst) .

2. **Generate Binary Files**
   - FW Binary generation
      - Navigate to **AXF/ELF TO BIN** → **Bin Conversion** in Astra SRSDK VSCode Extension
      - Load the generated `sr110_cm55_fw.elf` or `sr110_cm55_fw.axf` file
      - Click **Run Image Generator** to create the binary files
   - Model Binary generation (to place the Model in Flash)
      - To generate `.bin` file for TFLite models, please refer to [Vela compilation guide](Astra_SRSDK_vela_compilation_tflite_model.md).

3. **Flash the Application**

   **For WQVGA resolution:**
   - Flash the generated `B0_flash_full_image_GD25LE128_67Mhz_secured.bin` file directly to the device. Note: Model weights is placed in SRAM.

   **For VGA resolution:**

   - For VGA resolution, flash the **model binary first**, and then proceed to flash the **generated use case binary**.

   - **Steps:**
   1. Flash the pre-generated model binary: `person_segmentation(448x640).bin`.
      Due to memory constraints, the model weights need to be stored in Flash.
      Browse and select this binary from the following location and use **"Burn file to flash"**.
      Enter the specified flash address in the **"Flash Offset"** field and start flashing.
      - **Location:** `examples/vision_examples/uc_person_segmentation/models/`
      - **Flash address:** `0x629000`
      - **Calculation Note:**
         The flash address is determined by adding the `host_image` size and the `image_offset_SDK_image_B_offset` parameter (defined in `NVM_data.json`).
         Ensure the resulting address is aligned to a sector boundary (a multiple of 4096 bytes).
         This calculated address should then be assigned to the `image_offset_Model_A_offset` macro in your `NVM_data.json` file.

   2. Flash the generated `B0_flash_full_image_GD25LE128_67Mhz_secured.bin` file.

   Refer to the [Astra SRSDK VSCode Extension User Guide](../developer_guide/SRSDK_VSCode_Extension_Userguide.rst) for detailed instructions on flashing.

4. **Device Reset**
   Reset the target device after flashing is complete.

### Note:

The placement of the model (in **SRAM** or **FLASH**) is determined by its memory requirements. Models that exceed the available **SRAM** capacity, considering factors like their weights and the necessary **tensor arena** for inference, will be stored in **FLASH**.

### Running the Application

1. **Connect to Serial Interface**  
   - Connect to the newly enumerated COM port  
   - For logging output, connect to DAP logger port  

   ![Serial Connection](../_static/Assets/Images/user_guide/person_segmentation/image_1.png)

2. **The example logs will then appear in the logger window.**  

   ![Usecase Logs](../_static/Assets/Images/user_guide/person_segmentation/image_2.png)

3. **Next, navigate to Tools → Video Streamer in SynaToolkit to run the application.**  

   ![Tools - Video Streamer](../_static/Assets/Images/user_guide/person_segmentation/image_3.png)

4. **Video Streamer**  
   - Configure the following settings:  
     - **UC ID**: PERSON_SEGMENTATION
     - **RGB Demosaic**: BayerRGGB  

   ![Video Streamer Settings](../_static/Assets/Images/user_guide/person_segmentation/image_4.png)

   - Click **Create Usecase**  
   - Connect the image source  
   - Click **Start Usecase** to begin real-time segmentation  

   ![Usecase Running](../_static/Assets/Images/user_guide/person_segmentation/image_5.png)

5. **After starting the use case, Person segmentation will begin streaming video as shown below.**
   ![Usecase Running](../_static/Assets/Images/user_guide/person_segmentation/image_6.png)
