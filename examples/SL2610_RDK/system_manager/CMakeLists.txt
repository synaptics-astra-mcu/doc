#SRSDK examples - system_manager - CMakeLists.txt

if(CONFIG_MODULE_SYSTEM_MANAGER_ENABLED)
    
    if(${BUILD} STREQUAL "EXAMPLE")
        add_subdirectory(boot)
    
        target_sources(${TARGET} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/main.c
        )
        target_include_directories(${TARGET} PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/config
        )

        set(SEC_LIB_FILES
        "${INSTALL_ROOT}/prebuilt/seclib/libs/libseclib.a"
        CACHE STRING "seclib library"
        )

        list(APPEND SEC_LIB_FILES
            "${INSTALL_ROOT}/prebuilt/seclib/libs/libpsacrypto.a"
        )

        set(SEC_LIB_FILES "${SEC_LIB_FILES}" CACHE STRING "seclib library")

    else()
        target_include_directories(sdk_includes INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/config>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/boot>
        )
    endif()
endif()
