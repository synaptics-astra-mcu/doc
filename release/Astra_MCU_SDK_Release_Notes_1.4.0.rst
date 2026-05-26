Introduction 
============

   The Astra SR MCU from Synaptics is a cutting-edge, high-performance
   microcontroller designed to bring advanced AI and vision capabilities
   to a wide range of applications. The Astra SR MCU seamlessly
   integrates powerful processing, sophisticated vision systems, and
   exceptional energy efficiency into a compact package.

   Harness the power of artificial intelligence with the Astra SR MCU
   family, featuring scalable multi-core architectures tailored for both
   AI-enabled and non-AI use cases. AI-capable variants integrate an
   Arm® Cortex®-M55 CPU running up to 400 MHz alongside an Ethos™-U55
   micro NPU, delivering robust performance for real-time inferencing
   and tiered processing.

   The Astra MCU portfolio also includes SL2610-based configurations
   with a Cortex-M52 (CM52) running up to 200 MHz, designed for
   high-performance control and signal-processing workloads without AI
   acceleration.

   Across the Astra MCU family, the platform is equipped with a
   comprehensive set of interfaces to communicate with system components
   such as the application processor (AP), camera sensors, digital
   microphones, and other peripherals, enabling flexible integration
   across a wide range of embedded applications.

   The Astra MCU SDK 1.4.0 is a software development kit that supports
   the SR110 and SL2610 MCUs. It provides a comprehensive collection of
   source code, tools, and documentation, and is built on CMSIS. The SDK
   can be compiled using GCC, Arm Compiler 6 (AC6), and LLVM for SR110,
   while SL2610 supports compilation using GCC only. The high-level
   components included in this SDK are described below.

-  Free RTOS

-  Source codes of various peripheral drivers, image processing, low
   power sensing blocks.

-  SR110-specific reference applications demonstrating camera-based
   sensing and wake-up, person detection, classification, pose
   detection, segmentation, and hand gesture recognition. It also
   features JPEG pre-roll with frame transfer over USB CDC to a host PC
   or via SPI to an SPI-capable receiver, along with audio processing
   use cases supported on the SR110 platform.

-  Astra MCU SDK VSCode Extension that supports build and image
   generation, along with integrated video streaming capabilities.

-  Related SDK Documents

General 
=======

====================== ==================================
   **Date**               10th April 2026
====================== ==================================
   Version number         1.4.0
   Supported SoCs         SR110 Rev. B (B0) FCCSP & WLCSP
                       
                          SL2610
   Supported Platforms    SR110 RDK Board Rev A/B/C/D/E
                       
                          SL2610 RDK Board Rev B/C
   File name           
   APBL                   SR110: APBL v012F
                       
                          SL2610: 1.4.0
   SPK                    SR110: SPK RC4.0
   VS Code                1.4.3
====================== ==================================

Supported Peripherals
=====================

==================== ===================== =====================
   ** HW Component**    **CM55-B0**           **CM52**
==================== ===================== =====================
   UART                 √                     √
   IOMUX                √                     √
   GPIO                 √                     √
   DMA                  √ Memory to Memory    √ Memory to Memory
   I2C-S/M              √                     √
   Cache                √                     √
   Watchdog             √                     √
   Clocks               √                     √
   Timers               √                     √
   LP-Sense             √                     √
   USB 2.0 Device       √                     √
   SPI-S/M           √                     √(SPI-S)
   I2S M                √                     √
   xSPI (1/4 bits)      √                     √
   MIPI CSI RX -0       √                     X
   MIPI CSI RX -1       X                     X
   DVP Serial           √                     X
   DVP Parallel         X                     X
   SDIO                 √                     X
   SPWM                 X                     √
   I3C                  X                     √
==================== ===================== =====================

Supported Software Components
=============================

================== =========== ========
**SW Component**   **CM55-B0** **CM52**
================== =========== ========
Free RTOS          √           √
DMEM Service       √           √
Logger Service     √           √
USB Tiny USB Stack √           √
Flash Services     √           √
Host API Service   √           √
================== =========== ========

Supported Power Modes
=====================

+-----------------+------------------------+------------------------+
| **Mode**        |    **CM55-B0**         |    **CM52**            |
+=================+========================+========================+
| **Active**      | Full Operational mode: | Active Sleep: TBD      |
|                 | Gear1 (G1), Gear2      |                        |
|                 | (G2), Gear3 (G3)       |                        |
|                 |                        |                        |
|                 | Mode Entry: boot       |                        |
|                 | process                |                        |
|                 |                        |                        |
|                 | Mode Exit: N/A         |                        |
+-----------------+------------------------+------------------------+
| **Hibernation** | All memories: in       | Deep Sleep partial     |
|                 | retention              | retention: TBD         |
|                 |                        |                        |
|                 | Gear 3: (HP Proc) -    | Deep Sleep Mem         |
|                 | disabled               | retention: TBD         |
|                 |                        |                        |
|                 | Gear 2: (LP Proc) -    | Standby: TBD           |
|                 | disabled               |                        |
|                 |                        |                        |
|                 | Gear 1: (LP Sense) –   |                        |
|                 | disabled               |                        |
|                 |                        |                        |
|                 | Gear 0 (AON): awake    |                        |
|                 |                        |                        |
|                 | Mode entry: Host API’s |                        |
|                 | power manager commands |                        |
|                 | (0x1-0x4)              |                        |
|                 |                        |                        |
|                 | Mode exit:             |                        |
|                 | internal/external      |                        |
|                 | event (timer, GPI)     |                        |
+-----------------+------------------------+------------------------+

VS Code Extension
=================

================ =========== ========
** SW Tool**     **CM55-B0** **CM52**
================ =========== ========
Code Development √           √
Debug            √           √
Image Generator  √           √
Image Flashing   √           √
Video Streamer   √           X
================ =========== ========

Reference Applications
======================

============================== ======================== ========
**Use Case**                   **CM55-B0 w/ Ethos U55** **CM52**
============================== ======================== ========
Vision: Person Detection       √                        X
Vision: Person Classification  √                        X
Vision: Person Pose Detection  √                        X
Vision: Person Segmentation    √                        X
Vision: Hand Gesture Detection √                        X
Vision: Pre-roll               √                        X
Audio: PDM to I2S              X                        X
============================== ======================== ========

Sample Applications
===================

+----------------------------+--------------------------+----------+
| ** Example**               | **CM55-B0 w/ Ethos U55** | **CM52** |
+============================+==========================+==========+
|    Inference - Single SRAM | √                        | X        |
|    NN Model                |                          |          |
+----------------------------+--------------------------+----------+
| Inference - Single Flash   | √                        | X        |
| NN Model                   |                          |          |
+----------------------------+--------------------------+----------+
| UART Sample Application    | √                        | X        |
| with flow control          |                          |          |
+----------------------------+--------------------------+----------+
| I2C Sample Application     | √                        | √        |
+----------------------------+--------------------------+----------+
| PINMUX Sample Application  | √                        | X        |
+----------------------------+--------------------------+----------+
| GPIO Sample Application    | √                        | X        |
+----------------------------+--------------------------+----------+
| CLK Sample Application     | √                        | X        |
+----------------------------+--------------------------+----------+
| SPI Sample Application     | √                        | X        |
+----------------------------+--------------------------+----------+
| SDIO Sample Application    | √                        | X        |
| (SDCARD & SDIO devices)    |                          |          |
+----------------------------+--------------------------+----------+
| FATFS Sample Application   | √                        | X        |
+----------------------------+--------------------------+----------+
| BT Sample Application      | X                        | X        |
+----------------------------+--------------------------+----------+
| System Manager             | X                        | √        |
+----------------------------+--------------------------+----------+
| DMA Memory to Memory       | √                        | √        |
| Sample Application         |                          |          |
+----------------------------+--------------------------+----------+
| SPWM Application           | X                        | √        |
+----------------------------+--------------------------+----------+

.. _section-1:

Supported Sensors and Configuration
===================================

   Sensors supported only with CM55-B0

+------------+-----------------+-----------------+-----------------+
| **Sensor** | **CSI           | **Parallel      | **Serial**      |
|            | configuration** | Configuration** |                 |
|            |                 |                 | **              |
|            |                 |                 | Configuration** |
+============+=================+=================+=================+
| OV2C       | -  SDR: 480x270 |                 |                 |
|            |    3FPS 160Mbps |                 |                 |
|            |    per lane,    |                 |                 |
|            |    lane count = |                 |                 |
|            |    1            |                 |                 |
|            |                 |                 |                 |
|            | -  SDR:         |                 |                 |
|            |    1920x1080    |                 |                 |
|            |    60FPS        |                 |                 |
|            |    1.5Gbps per  |                 |                 |
|            |    lane, lane   |                 |                 |
|            |    count = 1    |                 |                 |
+------------+-----------------+-----------------+-----------------+
| OV5647     | -  SDR: 480x270 |                 |                 |
|            |    30FPS        |                 |                 |
|            |    280Mbps per  |                 |                 |
|            |    lane, lane   |                 |                 |
|            |    count = 2    |                 |                 |
|            |                 |                 |                 |
|            | -  SDR: 640x480 |                 |                 |
|            |    60FPS        |                 |                 |
|            |    280Mbps per  |                 |                 |
|            |    lane, lane   |                 |                 |
|            |    count = 2    |                 |                 |
+------------+-----------------+-----------------+-----------------+
| K351       |    SDR: Serial  |                 |                 |
|            |    Camera       |                 |                 |
|            |    500x500 4FPS |                 |                 |
+------------+-----------------+-----------------+-----------------+

Supported Flash List
====================

All supported with CM55-B0 and CM52.

========== =================
Vendor     Flash Part Number
========== =================
Macronix   MX25U12843G
GigaDevice GD25LE128E
WINBOND    W25Q128JW
========== =================

Known Issues
============

===== ======= ========== ========
Issue Summary Components Severity
===== ======= ========== ========
\                        
===== ======= ========== ========

Revision History 
================

================ ======== ==============================
Last Modified    Revision    Description
================ ======== ==============================
   June 2025     0.1      Astra MCU SDK Release 1.0.0
   August 2025      0.2      Astra MCU SDK Release 1.1.0
   November 2025    0.3      Astra MCU SDK Release 1.2.0
   January 2026     0.4      Astra MCU SDK Release 1.3.0
   April 2026       0.5      Astra MCU SDK Release 1.4.0
================ ======== ==============================

Copyright

Copyright © 2024 Synaptics Incorporated. All Rights Reserved.

Trademarks

Synaptics; the Synaptics logo; are trademarks or registered trademarks
of Synaptics Incorporated in the United States and/or other countries.
All other trademarks are the property of their respective owners.

Notice

This document contains information that is proprietary to Synaptics
Incorporated (“Synaptics”). The holder of this document shall treat all
information contained herein as confidential, shall use the information
only for its intended purpose, and shall not duplicate, disclose, or
disseminate any of this information in any manner unless Synaptics has
otherwise provided express, written permission.

Use of the materials may require a license of intellectual property from
a third party or from Synaptics. This document conveys no express or
implied licenses to any intellectual property rights belonging to
Synaptics or any other party. Synaptics may, from time to time and at
its sole option, update the information contained in this document
without notice.

INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,” AND
SYNAPTICS HEREBY DISCLAIMS ALL EXPRESS OR IMPLIED

WARRANTIES, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR

PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY INTELLECTUAL
PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE

LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION

WITH THE USE OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER
CAUSED AND BASED ON ANY THEORY OF LIABILITY,

WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
AND EVEN IF SYNAPTICS WAS ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF
A TRIBUNAL OF COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF
DIRECT DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE
LIABILITY TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.

Ordering Information

For ordering information and a complete list of Synaptics' products,
contact your Synaptics sales representative. Visit our website at
www.synaptics.com to locate the Synaptics office nearest you.

Contact Us

Visit our website a\ `t www.synaptics.com
t <http://www.synaptics.com/>`__\ o locate the Synaptics office nearest
you.

   |image0|

.. |image0| image:: ./media/image3.jpg
   :width: 2.5in
   :height: 0.57292in
