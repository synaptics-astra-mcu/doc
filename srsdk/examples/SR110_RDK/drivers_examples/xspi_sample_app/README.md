# XSPI Driver Sample Application

## Description

The XSPI Driver sample application validates basic external flash operations on SR110 using the flash driver stack.

In practical terms, it checks that the board can communicate with the external flash correctly and move data reliably. The sample runs a sequence of flash tests and prints detailed results through the logger.

What this sample tests:
- **Flash initialization:** verifies flash interface bring-up.
- **JEDEC ID read:** confirms flash device detection and identification.
- **Erase/write/read flow:** erases a test region, writes a known pattern, reads it back, and verifies byte-by-byte integrity.
- **Read/write throughput:** measures transfer time for 512-byte test buffers in:
  - 1-bit SDR mode
  - 4-bit SDR mode

If any mismatch is detected, the sample logs exact error locations (up to a limited count) and reports pass/fail for each test phase.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **xspi_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_xspi_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
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
3. Flash the firmware image:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```

---

## Running the Application using VS Code Extension

1. Press **RESET** on the board after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. XSPI sample logs appear in the logger window, including JEDEC ID, throughput timing, and data verification status.

**Seleted Application**

```
0391470066:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000023:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004210:[0][INF][SYS ]:------------------------------------------
0000007576:[0][INF][SYS ]:            Hello  ASTRA
0000010943:[0][INF][SYS ]:------------------------------------------
0000014309:[0][INF][SYS ]:System initialization done
0000016910:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000019385:[0][INF][FLSH]:Starting XSPI Sample Application...
0000022438:[0][INF][FLSH]:Starting Flash Initialization Test...
0000025569:[0][INF][FLSH]:[FLASH] Init successful
0000028029:[0][INF][FLSH]:Flash Initialization Test Passed.
0000030969:[0][INF][FLSH]:Starting Read Device ID Test...
0000033822:[0][INF][FLSH]:[FLASH] JEDEC ID: 0x1860C8 (MFG=0xC8)
0000036955:[0][INF][FLSH]:Read Device ID Test Passed.
0000039607:[0][INF][FLSH]:Starting Read/Write Throughput Test...
0000042786:[0][INF][FLSH]:Flash RW throughput test started
0000045678:[0][INF][FLSH]:Testing throughput: IO=1-bit, Mode=SDR
0000048858:[0][INF][FLSH]:[FLASH] RW throughput test (IO=1-bit, Mode=SDR)
0000052480:[0][INF][FLSH]:Configuring XSPI instance 0 for DIRECT mode with IO=1, DTR=0
0000056735:[0][INF][FLSH]:flash_erase: addr=0x0, len=0x200
0000059628:[0][INF][XSPI]:Erase 4k @ 0x0
0000090088:[0][INF][FLSH]:[FLASH] Write: 512 bytes in 0 ms
0000093318:[0][INF][FLSH]:[FLASH] Read : 512 bytes in 0 ms
0000096222:[0][INF][FLSH]:[FLASH] RW throughput test passed
0000099164:[0][INF][FLSH]:Throughput test PASSED: IO=1-bit, Mode=SDR
0000102536:[0][INF][FLSH]:Testing throughput: IO=4-bit, Mode=SDR
0000105716:[0][INF][FLSH]:[FLASH] RW throughput test (IO=4-bit, Mode=SDR)
0000109339:[0][INF][FLSH]:Configuring XSPI instance 0 for DIRECT mode with IO=4, DTR=0
0000113593:[0][INF][FLSH]:flash_erase: addr=0x0, len=0x200
0000116469:[0][INF][XSPI]:Erase 4k @ 0x0
0000145651:[0][INF][FLSH]:[FLASH] Write: 512 bytes in 1 ms
0000148742:[0][INF][FLSH]:[FLASH] Read : 512 bytes in 0 ms
0000151647:[0][INF][FLSH]:[FLASH] RW throughput test passed
0000154588:[0][INF][FLSH]:Throughput test PASSED: IO=4-bit, Mode=SDR
0000157960:[0][INF][FLSH]:Flash RW throughput test completed, final status=0
0000161715:[0][INF][FLSH]:Read/Write Throughput Test Passed.
0000164702:[0][INF][FLSH]:All Flash Tests Completed Successfully.
0000167954:[0][INF][FLSH]:XSPI Sample Application Completed Successfully.
```