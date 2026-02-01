if(CONFIG_XSPI_SAMPLE_APP)
    if(${BUILD} STREQUAL "EXAMPLE")
        target_sources(${TARGET} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/xspi_sample_app.c
            ${CMAKE_CURRENT_LIST_DIR}/main.c
        )

        target_include_directories(${TARGET} PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/config
        )
    else()
        target_include_directories(sdk_includes INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/config>
        )
    endif()
endif()
