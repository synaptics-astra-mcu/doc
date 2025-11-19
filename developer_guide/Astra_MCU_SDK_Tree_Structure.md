# MCU SDK Directory Structure

This document provides a comprehensive overview of the MCU SDK directory organization, including libraries, examples, tools, and documentation layout.


## Astra MCU SDK Directory Tree Structure

The layout below illustrates the structural and directory-level organization of Astra MCU SDK.

```
Astra MCU SDK
┣ 📂AI
┣    ┣ 📂infer_job
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂tflite_micro
┣    ┣    ┣ CMakeLists.txt
┣    ┣ CMakeLists.txt
┣    ┣ kconfig
┣ 📂arch
┣    ┣ 📂arm
┣    ┣    ┣ kconfig
┣    ┣ kconfig
┣ 📂Board
┣    ┣ 📂SR110_RDK
┣ 📂Bootloader
┣    ┣ 📂SR110
┣    ┣    ┣ 📂Bootloader
┣    ┣    ┣ 📂CMSIS
┣    ┣    ┣ 📂config
┣    ┣    ┣ 📂device
┣    ┣    ┣ 📂drivers
┣    ┣    ┣ 📂include
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣    ┣ SR110_BOOTLOADER.cmake
┣    ┣ CMakeLists.txt
┣    ┣ syna_device.h
┣ 📂Build
┣ 📂docs
┣ 📂drivers
┣    ┣ 📂aon
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂cache
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂clk
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂crypto
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂dma
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂ethosu
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂ethosu_core_driver
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂gpio
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂hw_sem
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂i2c
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂i2s
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂image_proc
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂ipc
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂irq
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂lp_sense
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂mpu
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂pinmux
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂pmu
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂pwr_mng
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂sdio
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂spi
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂timer
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂uart
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂usb
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂watchdog
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ CMakeLists.txt
┣    ┣ kconfig
┣ 📂examples
┣    ┣ 📂docs
┣    ┣ 📂out
┣    ┣ 📂tools
┣    ┣ 📂SR110_RDK
┣    ┣    ┣ 📂configs
┣    ┣    ┣ 📂drivers_examples
┣    ┣    ┣    ┣ 📂clk_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂fatfs_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂gpio_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂i2c_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂pinmux_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂sdio_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂spi_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uart_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂inference_examples
┣    ┣    ┣    ┣ 📂inference_basic_flash_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂inference_basic_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂inference_multi_model_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂config
┣    ┣    ┣    ┣    ┣ freertosconfig.h
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂usb_examples
┣    ┣    ┣    ┣ 📂usb_cdc_camera_sensor_tx_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂usb_cdc_echo_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂usb_cdc_image_downloader_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂usb_cdc_rx_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂usb_cdc_tx_counter_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂usb_cdc_tx_sample_app
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂config
┣    ┣    ┣    ┣    ┣ freertosconfig.h
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂vision_examples
┣    ┣    ┣    ┣ 📂uc_person_detection
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uc_person_pose_detection
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uc_person_segmentation
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uc_image_stitching
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uc_hand_gesture_detection
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uc_jpeg_preroll
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂uc_person_classification
┣    ┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣    ┣ 📂config
┣    ┣    ┣    ┣    ┣ freertosconfig.h
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ CMakeLists.txt
┣    ┣ kconfig
┣ 📂off_chip_components
┣    ┣ 📂img_sensors
┣    ┣    ┣ 📂common
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ 📂components
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂h63
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂k251
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂k351
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂ov02c10
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ 📂ov5647
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ kconfig
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂io_expander
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂xspi_flash
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ CMakeLists.txt
┣    ┣ kconfig
┣ 📂os
┣    ┣ 📂freertos
┣    ┣ CMakeLists.txt
┣ 📂prebuilt
┣ 📂soc
┣    ┣ 📂SR110
┣    ┣    ┣ 📂cm55
┣    ┣    ┣    ┣ 📂config
┣    ┣    ┣    ┣    ┣ sr110_b0_cm55_hw_s.sct
┣    ┣    ┣    ┣    ┣ sr110_cm55_hw_gcc_arm.ld
┣    ┣    ┣    ┣ 📂include
┣    ┣    ┣    ┣ 📂source
┣    ┣    ┣    ┣    ┣ startup_sr110_cm55.c
┣    ┣    ┣    ┣    ┣ system_sr110_cm55.c
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ 📂common
┣    ┣    ┣    ┣ 📂include
┣    ┣    ┣    ┣ 📂source
┣    ┣    ┣    ┣    ┣ sr110_clk.c
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ syna_version.h
┣    ┣    ┣ 📂config
┣    ┣    ┣    ┣ platform_device_config.h
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ SR110.cmake
┣    ┣ CMakeLists.txt
┣    ┣ kconfig
┣    ┣ syna_device.h
┣ 📂third_party
┣    ┣ 📂cmsis
┣    ┣ 📂cmsis-freertos
┣    ┣ 📂fatfs
┣    ┣    ┣ CMakeLists.txt
┣    ┣ CMakeLists.txt
┣ 📂tools
┣ 📂utilities
┣    ┣ 📂common
┣    ┣    ┣ 📂api
┣    ┣    ┣ 📂dev
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ 📂fw_error
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂dap
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂data_flow
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂dmem
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂flash_aux
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂fw_update
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂host_api
┣    ┣    ┣ 📂services
┣    ┣    ┣    ┣ 📂service_flash_aux
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_fw_update
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_gpio
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_i2s
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_img_sensor
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_logger
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_pinmux
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_pwr_mng
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_system
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_timer
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_uc_manager
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣    ┣ 📂service_watchdog
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂image_processing
┣    ┣    ┣ 📂post_process
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ 📂pre_process
┣    ┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂logger
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂mailbox
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂nvm
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂profiler
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂reg_logger
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂shell
┣    ┣    ┣ CMakeLists.txt
┣    ┣ 📂syna_service
┣    ┣    ┣ CMakeLists.txt
┣    ┣    ┣ kconfig
┣    ┣ 📂sys_event
┣    ┣    ┣ CMakeLists.txt
┣    ┣ CMakeLists.txt
┣    ┣ kconfig
┣ CMakeLists.txt
┣ kconfig
```
