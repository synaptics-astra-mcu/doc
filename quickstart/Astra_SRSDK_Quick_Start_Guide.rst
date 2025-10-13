Getting Started SR110
=====================

The Astra SRSDK is a comprehensive software development kit tailored
to harness the exceptional capabilities of the SR series of MCUs. The
Astra SRSDK is designed to cater to the Vision and AI capabilities of
the Astra SR MCU.

To facilitate a seamless development journey, the Astra SRSDK includes
the following key components:

-  Optimized libraries and frameworks specifically tuned for the SR
   series hardware accelerators.

-  Sample Applications - Demonstrations real-world use cases such as
   vision-based low power AI.

-  Documentation and Tutorials - Detailed resources guiding developers
   from initial setup to advanced optimization techniques.

-  VSCode Extension

Prerequisites
-------------

-  Windows or Linux

-  VS Code

-  Astra Machina Micro Kit

Hardware Setup
--------------

1. Unbox the evaluation kit and visually inspect it for damage.

2. Ensure that all switches and jumpers are set to their default
   settings. Refer to the Astra Machina Micro Reference Manual for
   details.

3. Connect the Debug IC USB port to your host PC using the supplied USB
   cable. In the figure below, this is the bottom left connector

   .. raw:: html

      <figure>

   .. raw:: html

      <figcaption>

   Figure 1. SR110 RDK Board

   .. raw:: html

      </figcaption>

   .. raw:: html

      </figure>

Hardware Connections
--------------------

DAP Connections
~~~~~~~~~~~~~~~

This section provides guidance on how to connect to the DAP chip of the
SR110 RDK.

1. Connect the RDK (Type-C connector on the left in Figure 1) and the PC
   (type A) using the USB cable.
2. Wait a few seconds for the DAP to enumerate for the first time (up to
   15 seconds).
3. When the DAP chip is plugged in, two USB Serial Devices will appear
   (COM numbers may vary; e.g., COM48 and COM49 in the example below).
   These correspond to the CDC 0 and CDC 1 interfaces. One is used for
   firmware updates and the other for logging. Try both ports to
   determine which one is designated for firmware updates. Take note of
   the COM ports, as they will be used later during the flashing and
   debugging of the RDK.

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Install VS Code Extension
-------------------------

1. In VS Code open a new terminal window. Type the following command

..

   > code –install-extension <path to extension>.vsix

Note the extension is located in the SDK here /tools/vscode_extension

2. Reload VS Code.

Install Required Tools
----------------------

1. Click on the Synaptics Extension icon

2. Navigate to Install Tools:

3. Follow the prompts to install all required tools for use with the
   SDK. Note that during install the system may prompt you to approve
   installation of some tools, so pay attention during this time.

Import the SDK
--------------

1. Within the Synaptics Extension go to Import SDK and chose the
   top-level folder for the SDK which you installed on your system.

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Build Sample Application
------------------------

1. In the Synaptics Extension navigate to Imported Repos and select
   Build or Clean SDK.

2. Choose

   -  sr110_cm55_fw

   -  Debug

   -  sr110_rdk

   -  GCC

   -  demo_sample_app.

3. Press Build

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Create bin File for Flashing
----------------------------

Once your application has been built a .elf file will be created. This
needs to be converted into a .bin file to load onto the onboard flash on
the Astra Machina Micro board.

1. In the Synaptics Extension navigate to AXF/ELF to BIN and select Bin
   Conversion

2. The .elf file created when you built the demo app should be
   automatically populated. If not click browse and navigate to the .elf
   file which is in the out folder of the SDK

3. Select Flash Image and Flash Type of GD25LE128, and select Secured
   Image

4. Press Run Image Generator

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Update Debug IC Firmware
------------------------

The Astra Machina Micro board comes with an onboard Debug IC. The FW for
this debug IC could be out of date so it is best to update it.

1. In the Synaptics Extension go to Image Loading and Image Flashing

2. Select Service Type of FW

3. Select COM Port. When you plugged USB into the Astra Machina Micro
   Debug IC USB Port two COM Ports appeared. Select one of them to try
   first, if it fails then try the other. > Note: In linux the available
   CDC will appear as /dev/ttyACM0, /dev/ttyACM1 etc. Use these for
   flashing in FW mode (FW update).

4. Browse to the location of the .bin for the debug IC which is in
   tools/cmsis_dap/cmsis_dap.bin

5. Press Execute

6. Once flashing is complete unplug and replug the USB cable

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Flash Application Image
-----------------------

1. From the Image Flashing dialog select Service Type of SWD/JTAG

2. Select Adapter Driver as CMSIS-DAP

3. Navigate to the .bin file you previously created. This is typically
   stored in the user directory in a folder called Bin_Location.

4. Press Flash Execute

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Run Application
---------------

1. After programming finishes un-plug and re-plug the USB cable into the
   Debug IC USB connector

2. Open a terminal application such as the Serial Monitor in VSCode or
   Tera Term

3. Select the COM port that wasn’t the FW port and connect to it

4. You should now see the demo printing out info:

0119215263:[0][INFO][SYS]:Task vTaskDemo1

0119715263:[0][INFO][SYS]:Task vTaskDemo1

0120115263:[0][INF][SYS]:Task vTaskDemo2

0120215263:[0][INF][SYS]:Task vTaskDemo1

0120715263:[0][INF][SYS]:Task vTaskDemo1

0121115263:[0][INF][SYS]:Task vTaskDemo2

Debug Application
-----------------

If you want, you can debug the application as well

1. In the Synaptics Extension go to Debug Probes and Debug Probe
   Interface

2. Select the .elf for your application (if not already selected)

3. Choose CMSIS DAP as the adapter driver

4. Press Download & Reset Program

A debug session is started and the application halts at the start of
main

.. raw:: html

   <figure>

.. raw:: html

   </figure>

Running Vision AI Applications
------------------------------

The first part walked you through how to setup your system and run a
simple demo application. Now it is time to run a vision AI example.

Follow the steps from above except this time choose something like
uc_person_detection. Follow the same steps to build and flash.

Once the image is flashed plug another USB cable into the 2nd USB port
on the Astra Machina Micro board.

Updating Drivers
~~~~~~~~~~~~~~~~

This step is important to enable image presentation via SynaToolkit, you
are required to configure comport with “Zadig”.

1. Download the Zadig USB driver from the following link:
   https://zadig.akeo.ie/

2. Open zadig-2.8.exe

3. In the “Options” tab choose “List All Devices”

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

4. In the drop-down list choose “SR 100-B0 CDC 1”

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

5. Click on “Replace Driver”

   .. raw:: html

      <figure>

   .. raw:: html

      </figure>

Installing SynaTool Kit
~~~~~~~~~~~~~~~~~~~~~~~

Located in the tools directory is a SynaToolKit_x.y.z.exe. If you are a
windows user install this tool.

Running Vision AI in SynaTool Kit.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Go to the README.md file for the chosen Vision AI use case and follow
the directions under the heading “Running the Application.”
