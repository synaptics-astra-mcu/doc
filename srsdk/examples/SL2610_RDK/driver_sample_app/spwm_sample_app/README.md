# SPWM Driver Application

## Description

The SPWM driver application provides a configurable PWM framework for generating precise and advanced pulse-width modulated waveforms. It supports multiple PWM operating modes such as Normal PWM, PWM with Dead-Time (PWM_DT), and Pseudo-Random PWM (PWM_PR). The driver enables accurate control of frequency, duty cycle, dead-time, trigger routing, interrupt generation, and descriptor-based burst operation.

The SPWM driver is intended for motor control, power electronics, and timing-critical embedded applications. It supports both continuous operation and descriptor-controlled burst generation, allowing waveform execution with minimal CPU intervention.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SL2610 guides and VS Code Extension guide:
- [SL2610 Platform Guide](../../../../docs/SL2610/SL2610_Platform_Guide.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select `cm52_spwm_sample_app_rdk` in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

![Build Configurations](assets/vs_spwm_sl2610.png)

**Flash/Image Generation (VS Code):**
1. Build the SL2610 bootloader image.
2. Generate output binaries (equivalent of `make imagegen`) and collect:
   - `sl2610_bootloader_extras.bin`
   - `sl2610_bootloader_output.bin`
   - `sl2610_cm52_fw_extras.bin`
   - `sl2610_cm52_fw_output.bin`
3. Copy generated binaries to VSSDK.
4. Generate the system sub-image in VSSDK.
   - Refer: [SL2610 Platform Guide - Image Generation](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-generation-2)
5. Flash/download the MCU image to target.
   - Refer: [SL2610 Platform Guide - Image Flashing](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-flashing)

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SL2610 Platform Guide:
- [SL2610 Platform Guide](../../../../docs/SL2610/SL2610_Platform_Guide.md)

**Build (CLI):**
1. Configure SPWM mode selection in `spwm_sample_app.h` before building:
   ```bash
   #define SPWM_TIMER_MODE                     0
   #define SPWM_CAPTURE_MODE                   0
   #define SPWM_PWM_MODE                       0
   #define SPWM_PWM_DEADTIME_MODE              0
   #define SPWM_PWM_PSEUDO_RANDOM_MODE         0
   #define SPWM_PWM_MODE_DESC                  0

   /* DESCRIPTOR MODE CONFIGURATION */
   #if SPWM_PWM_MODE_DESC
      /* Demo selection - set ONE to 1, rest to 0 */
      #define SPWM_DEMO_440_480KHZ            0
      #define SPWM_DEMO_800_840KHZ            0
      #define SPWM_DEMO_100_400KHZ            0
      #define SPWM_DEMO_200_800KHZ            0
   #endif
   ```
   **Note:** Enable only one mode at a time.
2. Build the SL2610 bootloader from SDK root:
   ```bash
   make sl2610_bootloader_rdk_defconfig BOARD=SL2610_RDK
   make
   ```
3. Build SPWM sample app from `<sdk-root>/examples`:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm52_spwm_sample_app_rdk_defconfig BOARD=SL2610_RDK BUILD=SRSDK
   ```

**Image Generation and Flash (CLI):**
1. Generate binaries:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```
2. Copy generated binaries to VSSDK:
   - `sl2610_bootloader_extras.bin`
   - `sl2610_bootloader_output.bin`
   - `sl2610_cm52_fw_extras.bin`
   - `sl2610_cm52_fw_output.bin`
3. Generate system sub-image in VSSDK.
   - Refer: [SL2610 Platform Guide - Image Generation](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-generation-2)
4. Flash/download image to target.
   - Refer: [SL2610 Platform Guide - Image Flashing](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-flashing)

---

## Running the Application using VS Code Extension

1. Power the board and press **RESET** after flashing.
2. For logging output, connect UART to the target and open a serial console (for example, MobaXterm).
3. For PWM waveform observation, connect a logic analyzer to the required pins.

![Setup](assets/j32_pinout.png)

### Sample applications logs and output

1. **Timer Application**
   - Console logs.
   ```bash
   0000000000:[0][INF]▒▒0▒J:SPWM Timer Mode Starts (group 0)
   0000000005:[0][INF]▒▒0▒J:CLKDIV[0] Setup: int_div=1 frac_div=0
   0000000011:[0][INF]▒▒0▒J:INTERRUPT Setup: group=0 intr=1
   0000000016:[0][INF]▒▒0▒J:[ISR] Callback triggered!
   0000000016:[0][INF]▒▒0▒J:[ISR] Group 0: CC0 match
   0000000016:[0][INF]▒▒0▒J:[ISR] Group 0: CC0 match
   0000000022:[0][INF]▒▒0▒J:CC0 interrupt received, counter=3034
   0000000028:[0][INF]▒▒0▒J:SPWM-Timer Sample App Completed (group 0)!
   ```

2. **Timer + Capture Application**
   - Console logs.
   ```bash
   0000000000:[0][INF]▒▒0▒K:SPWM Timer + Capture App Starts
   0000000005:[0][INF]▒▒0▒K:CLKDIV[0] Setup: int_div=1 frac_div=0
   0000000011:[0][INF]▒▒0▒K:INTERRUPT Setup: group=0 intr=1
   0000000016:[0][INF]▒▒0▒J:[ISR] Callback triggered!
   0000000016:[0][INF]▒▒0▒K:[ISR] Group 0: CC0 match
   0000000022:[0][INF]▒▒0▒K:Capture results: CC0=1108 CC1=1108
   0000000027:[0][INF]▒▒0▒K:Capture successful
   0000000031:[0][INF]▒▒0▒K:SPWM TIMER + CAPTURE Sample App Completed!
   ```

3. **PWM Sample Application**
   - Console logs.
   ```bash
   0000000000:[0][INF]▒▒0▒G:SPWM PWM-Demo Starts
   0000000004:[0][INF]▒▒0▒G:CLKDIV[0] Setup: int_div=1 frac_div=0
   0000000010:[0][INF]▒▒0▒G:SPWM Group 0 Enabled
   0000000014:[0][INF]▒▒0▒G:PWM started: Period=5000 CC0=1000
   0000000020:[0][INF]▒▒0▒G:Duty = 20.0%
   0000000023:[0][INF]▒▒0▒G:PWM Sample App Completed Successfully!
   ```

   ![PWM Output](assets/pwm.png)

4. **PWM with Deadtime Application**
   - Console logs.
   ```bash
   0000000000:[0][INF]▒▒0▒G:PWM-DT App Starts
   0000000003:[0][INF]▒▒0▒G:CLKDIV[0] Setup: int_div=1 frac_div=0
   0000000010:[0][INF]▒▒0▒G:PWM-Deadtime Sample App Completed!
   ```

   ![PWMDT Output](assets/pwmdt.png)

5. **PWM Pseudo Random Application**
   - Console logs.
   ```bash
   0000000000:[0][INF]▒▒0▒I:SPWM PWM Pseudo Random Mode Test Starts
   0000000005:[0][INF]▒▒0▒I:CLKDIV[0] Setup: int_div=1 frac_div=0
   0000000012:[0][INF]▒▒0▒I:INTERRUPT Setup: group=0 intr=1
   0000000017:[0][INF]▒▒0▒I:PWM Pseudo Random Mode Sample App Completed!
   ```

   ![PWMPR Output](assets/pwmpr.png)

6. **PWM Ramp signal with descriptors Application**
   - Console logs.
   ```bash
   0000000000:[0][INF]▒▒0▒R:  Step 0: Period=999 CC0=200 Cycles=1 (f=100000 Hz, duty=20.00%, ~11.2µs)
   0000000008:[0][INF]▒▒0▒R:  Step 1: Period=699 CC0=140 Cycles=1 (f=142857 Hz, duty=20.00%, ~11.2µs)
   0000000017:[0][INF]▒▒0▒R:  Step 2: Period=537 CC0=107 Cycles=2 (f=185874 Hz, duty=19.89%, ~11.2µs)
   0000000026:[0][INF]▒▒0▒R:  Step 3: Period=436 CC0=87 Cycles=2 (f=228833 Hz, duty=19.91%, ~11.2µs)
   0000000035:[0][INF]▒▒0▒R:  Step 4: Period=367 CC0=73 Cycles=3 (f=271739 Hz, duty=19.84%, ~11.2µs)
   0000000044:[0][INF]▒▒0▒R:  Step 5: Period=317 CC0=63 Cycles=3 (f=314465 Hz, duty=19.81%, ~11.2µs)
   0000000053:[0][INF]▒▒0▒R:  Step 6: Period=279 CC0=56 Cycles=4 (f=357143 Hz, duty=20.00%, ~11.2µs)
   0000000062:[0][INF]▒▒0▒R:  Step 7: Period=249 CC0=50 Cycles=4 (f=400000 Hz, duty=20.00%, ~11.2µs)
   0000000071:[0][INF]▒▒0▒R:PWM ramp: 8 steps, 20 total cycles, ~90 µs burst, 58 descriptors
   0000000079:[0][INF]▒▒0▒R:CLKDIV[0] Setup: int_div=1 frac_div=0
   0000000085:[0][INF]▒▒0▒R:Initial config: ACTIVE=[Period=10 CC1=2] BUFFER=[Period=999 CC1=200]
   0000000093:[0][INF]▒▒0▒R:Building descriptor chain for configured burst time
   0000000100:[0][INF]▒▒0▒R:Descriptor chain built with 58 descriptors
   0000000107:[0][INF]▒▒0▒R:Descriptor 0 completed
   0000000108:[0][INF]▒▒0▒R:PWM-Ramp started with the configured frequency and duty cycle
   0000000116:[0][INF]▒▒0▒R:PWM-Ramp using Descriptor completed successfully!
   ```

   ![PWM with descriptor Output](assets/pwm_descriptor.png)
