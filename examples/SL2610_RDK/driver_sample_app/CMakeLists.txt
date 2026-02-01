#SL2610_RDK Sample Applications - Drivers CMakeLists.txt

if(CONFIG_DRIVER_SAMPLE_APP_ENABLED)
    add_subdirectory(dma_sample_app)

    add_subdirectory(xspi_sample_app)

    add_subdirectory(i2c_exp_app)

    add_subdirectory(spwm_sample_app)
endif()
