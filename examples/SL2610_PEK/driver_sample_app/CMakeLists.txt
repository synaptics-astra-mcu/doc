#SL2610_PEK Sample Applications - Drivers CMakeLists.txt

if(CONFIG_DRIVER_SAMPLE_APP_ENABLED)
    add_subdirectory(dma_sample_app)

    add_subdirectory(i2c_sample_app)

    add_subdirectory(emmc_sample_app)

    add_subdirectory(i2c_scan_sample_app)

    add_subdirectory(i2c_exp_app)

    add_subdirectory(spwm_sample_app)
endif()
