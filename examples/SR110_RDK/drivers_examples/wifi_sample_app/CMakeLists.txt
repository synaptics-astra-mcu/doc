if(CONFIG_WIFI_SAMPLE_APP)
    if(${BUILD} STREQUAL "EXAMPLE")
        target_sources(${TARGET} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/main.c
            ${CMAKE_CURRENT_LIST_DIR}/wifi_sample_app.c
            ${CMAKE_CURRENT_LIST_DIR}/sdio_sample_app.c
        )

        target_include_directories(${TARGET} PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/config
        )

        set(MHD_LIB  ${INSTALL_ROOT}/prebuilt/wifi/libs/libmhd.a CACHE STRING "Wifi lib")
        message(STATUS "Using Wifi MHD library")
        set(WL_LIB  ${INSTALL_ROOT}/prebuilt/wifi/libs/libmhd_test_wl.a CACHE STRING "WL lib")
        message(STATUS "Using Wifi WL library")

    else()
        target_include_directories(sdk_includes INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/config>
        )
    endif()
endif()
