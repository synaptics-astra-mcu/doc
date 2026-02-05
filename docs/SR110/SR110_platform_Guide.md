# SR110 Platform Guide

This document provides SR110-specific hardware setup and platform notes. For build, image conversion, flashing, and debugging workflows, use:

- [SR110 Build and Flash with CLI](./SR110_Build_and_Flash_with_CLI.md)
- [SR110 Build and Flash with VS Code](./SR110_Build_and_Flash_with_VSCode.md)

---

## Platform Overview

### SR110 Specifications

**Processor:** Arm Cortex-M55 CPU

**Memory:** External flash (e.g., GD25LE128)

**Supported Development Kits:** **SR110_RDK** (Astra Machina Micro SR Series - Rev B/C/E)

---

## Hardware Setup

### Astra Machina Micro (SR110 RDK)

<figure>
<img src="../Assets/Images/media/RDK_REVB_main image.png" style="width:3.39653in;height:2.89792in" alt="SR110 RDK" />
<figcaption><b>Figure 1.</b> SR110 RDK Board</figcaption>
</figure>

### Connection Steps

1. **Power and Debug Connection (J14)**
   - Connect **Debug IC USB (J14)** to your host.
   - This single connection provides:
     - Board power
     - Debug interface (CMSIS-DAP)
     - Serial console (CDC COM port)

2. **Optional USB CDC streaming (J13)**
   - Some vision examples use **J13** for USB CDC streaming.
   - Keep **J14** connected for power and debug.

3. **Verify Default Configuration**
   - Confirm jumpers and switches are in default positions if needed.

### Identify COM Ports

- **Windows:** Two COM ports appear in Device Manager (Debug IC FW + serial console).
- **Linux:** `/dev/ttyACM0`, `/dev/ttyACM1`
- **macOS:** `/dev/tty.usbmodem*`

### Verify Hardware Setup

1. Confirm the board powers up when J14 is connected.
2. Verify the COM ports appear on the host.
3. Open a serial terminal (115200 baud, 8N1) and reset the board to see logs.

---

## References

- **VS Code workflows:** [Astra MCU SDK VS Code Extension User Guide](../Astra_MCU_SDK_VSCode_Extension_User_Guide.md)
- **CLI workflows:** [Astra MCU SDK User Guide](../Astra_MCU_SDK_User_Guide.md)

---

**Document Version:** 3.1
**Last Updated:** February 2026
**Supported Platforms:** SR110_RDK (Astra Machina Micro SR Series - Rev B/C/E)
