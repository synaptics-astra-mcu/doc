SR110 User Guide
*****************

This section provides comprehensive user guides and demo applications for the SR110 evaluation platform. Learn how to run AI vision applications, configure sensors, and explore the full capabilities of the SR110 board through hands-on examples.

**Download Binaries (Path)**: All demo binaries can be downloaded from the GitHub repository under `GitHub Releases <https://github.com/synaptics-astra-mcu/srsdk_demos/tree/v1.2.0>`_

**Download Binaries (.zip)**: The pre-compiled demo binaries (ZIP package) are available at `GitHub Releases <https://github.com/synaptics-astra-mcu/srsdk_demos/releases/tag/v1.2.0>`_


.. toctree::
   :maxdepth: 1

   ../srsdk/examples/sample_demo_app/README
   ../srsdk/examples/vision_examples/uc_person_classification/README
   ../srsdk/examples/vision_examples/uc_person_detection/README
   ../srsdk/examples/vision_examples/uc_person_segmentation/README
   ../srsdk/examples/vision_examples/uc_person_pose_detection/README
   ../srsdk/examples/vision_examples/uc_hand_gesture_detection/README
   ../srsdk/examples/vision_examples/uc_hand_gesture_hid_mouse/README
   ../srsdk/examples/vision_examples/uc_fid_hgd/README
   ../srsdk/examples/vision_examples/jpeg_sample_app/readme
   ../srsdk/examples/vision_examples/uc_jpeg_preroll/README
   ../srsdk/examples/audio_examples/uc_audio_mic/README
   ../srsdk/examples/driver_examples/clk_sample_app/README
   ../srsdk/examples/driver_examples/timer_sample_app/README
   ../srsdk/examples/driver_examples/dma_sample_app/README
   ../srsdk/examples/driver_examples/fatfs_sample_app/README
   ../srsdk/examples/driver_examples/gpio_sample_app/README
   ../srsdk/examples/driver_examples/i2c_sample_app/README
   ../srsdk/examples/driver_examples/pinmux_sample_app/README
   ../srsdk/examples/driver_examples/sdio_sample_app/README
   ../srsdk/examples/driver_examples/spi_sample_app/README
   ../srsdk/examples/driver_examples/uart_sample_app/README
   ../srsdk/examples/driver_examples/xspi_sample_app/README
   ../srsdk/examples/inference_examples/inference_basic_sample_app/README
   ../srsdk/examples/inference_examples/inference_basic_flash_sample_app/README
   ../srsdk/examples/inference_examples/inference_multi_model_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_camera_sensor_tx_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_echo_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_hid_mouse_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_image_downloader_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_rx_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_tx_counter_sample_app/README
   ../srsdk/examples/usb_examples/usb_cdc_tx_sample_app/README
   ../srsdk/examples/usb_examples/usb_hid_mouse_sample_app/README

**Sample Demo:**

:doc:`../srsdk/examples/sample_demo_app/README`
   **Sample Demo Application** - Demonstrates FreeRTOS task-creation and scheduling reference for SR110.

**AI Vision Demos:**

:doc:`../srsdk/examples/vision_examples/uc_person_classification/README`
   **Person Classification Demo** - Step-by-step guide to run person classification applications, including setup, configuration, and result interpretation.

:doc:`../srsdk/examples/vision_examples/uc_person_detection/README`
   **Person Detection Demo** - Complete tutorial for person detection functionality, covering camera setup, detection parameters, and output analysis.

:doc:`../srsdk/examples/vision_examples/uc_person_segmentation/README`
   **Person Segmentation Demo** - Detailed instructions for running person segmentation applications with real-time image processing capabilities.

:doc:`../srsdk/examples/vision_examples/uc_person_pose_detection/README`
   **Person Pose Detection Demo** - Comprehensive guide for pose detection applications, including keypoint detection and pose analysis features.

:doc:`../srsdk/examples/vision_examples/uc_hand_gesture_detection/README`
   **Hand Guesture Detection Demo** - A complete guide for hand guesture detection applications, perform real-time gesture recognition with hand detection, single hand tracking and hand-key point estimation.

:doc:`../srsdk/examples/vision_examples/uc_hand_gesture_hid_mouse/README`
   **Hand Guesture HID Mouse Demo** - A complete guide for hand guesture hid mouse applications, perform real-time gesture recognition with hand detection and control a host PC mouse over USB HID with hand gestures.

:doc:`../srsdk/examples/vision_examples/uc_fid_hgd/README`
   **Face Identification with Hand Gesture Detection Demo** - A complete guide for face identification + hand gesture detection applications, perform real-time face identification + gesture recognition with hand detection, single hand tracking and hand-key point estimation.

:doc:`../srsdk/examples/vision_examples/jpeg_sample_app/readme`
   **JPEG Sample Demo** - A complete guide for the JPEG Camera sample application, capture 648 × 368 RAW Bayer images and JPEG pre-roll frames using the PS5416, and stream the latest JPEG image every 3 seconds with output saved in the video_stream_output/overlayed_frames directory.

**Audio Example:**

:doc:`../srsdk/examples/audio_examples/uc_audio_mic/README`
   **Audio MIC Application demo** - Demonstrates routing microphone audio to the I2S interface and LP_Sense memory to validate cross-interface data compatibility

**Data Transfer & Communication:**

:doc:`../srsdk/examples/vision_examples/uc_jpeg_preroll/README`
   **JPEG Preroll Demo** - Guide for JPEG image capture and preroll functionality, including USB transfer and image processing workflows.

**Driver Examples:**

:doc:`../srsdk/examples/driver_examples/clk_sample_app/README`
   **Clock Demo** - Shows clock source configuration and peripheral clock setup for SR110 driver applications.

:doc:`../srsdk/examples/driver_examples/dma_sample_app/README`
   **DMA Demo** - Demonstrates direct memory access transfers for efficient data movement between memory and peripherals.

:doc:`../srsdk/examples/driver_examples/fatfs_sample_app/README`
   **FATFS Demo** - Covers FAT file system initialization, file operations, and storage access workflows.

:doc:`../srsdk/examples/driver_examples/gpio_sample_app/README`
   **GPIO Demo** - Explains GPIO pin configuration, input/output control, and basic digital signal handling.

:doc:`../srsdk/examples/driver_examples/i2c_sample_app/README`
   **I2C Demo** - Provides steps for I2C bus setup and read/write communication with connected devices.

:doc:`../srsdk/examples/driver_examples/pinmux_sample_app/README`
   **Pinmux Demo** - Describes pin multiplexing configuration to map board pins to required peripheral functions.

:doc:`../srsdk/examples/driver_examples/sdio_sample_app/README`
   **SDIO Demo** - Shows SDIO interface bring-up and data transfer flow for SDIO-capable devices.

:doc:`../srsdk/examples/driver_examples/spi_sample_app/README`
   **SPI Demo** - Details SPI master communication, transfer parameters, and example transaction sequences.

:doc:`../srsdk/examples/driver_examples/uart_sample_app/README`
   **UART Demo** - Demonstrates serial communication setup, transmit/receive operations, and console interaction.

:doc:`../srsdk/examples/driver_examples/xspi_sample_app/README`
   **XSPI Demo** - Explains extended SPI usage for external flash/peripheral access with high-speed transactions.

**Inference Examples:**

:doc:`../srsdk/examples/inference_examples/inference_basic_sample_app/README`
   **Basic Inference Demo** - Demonstrates a simple end-to-end model inference flow on SR110 using a single model pipeline.

:doc:`../srsdk/examples/inference_examples/inference_basic_flash_sample_app/README`
   **Flash-Based Inference Demo** - Shows how to load and run inference models stored in flash memory.

:doc:`../srsdk/examples/inference_examples/inference_multi_model_sample_app/README`
   **Multi-Model Inference Demo** - Covers running multiple inference models and managing model execution in one application.

**USB Examples:**

:doc:`../srsdk/examples/usb_examples/usb_cdc_camera_sensor_tx_sample_app/README`
   **USB CDC Camera Sensor TX Demo** - Demonstrates transmitting camera sensor data over USB CDC.

:doc:`../srsdk/examples/usb_examples/usb_cdc_echo_sample_app/README`
   **USB CDC Echo Demo** - Shows loopback/echo behavior for validating USB CDC communication.

:doc:`../srsdk/examples/usb_examples/usb_cdc_hid_mouse_sample_app/README`
   **USB CDC HID Mouse Demo** - Demonstrates a combined CDC and HID mouse interface for host interaction over USB.

:doc:`../srsdk/examples/usb_examples/usb_cdc_image_downloader_sample_app/README`
   **USB CDC Image Downloader Demo** - Provides image transfer workflow from host to device over USB CDC.

:doc:`../srsdk/examples/usb_examples/usb_cdc_rx_sample_app/README`
   **USB CDC RX Demo** - Covers receiving data streams on the device using USB CDC endpoints.

:doc:`../srsdk/examples/usb_examples/usb_cdc_tx_counter_sample_app/README`
   **USB CDC TX Counter Demo** - Demonstrates periodic counter-based data transmission over USB CDC.

:doc:`../srsdk/examples/usb_examples/usb_cdc_tx_sample_app/README`
   **USB CDC TX Demo** - Shows basic USB CDC transmit flow for sending application data to the host.

:doc:`../srsdk/examples/usb_examples/usb_hid_mouse_sample_app/README`
   **USB HID Mouse Demo** - Provides a simple USB HID mouse example for host-side pointer interaction.

