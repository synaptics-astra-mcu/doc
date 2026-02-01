#SL2610_RDK examples - spwm_sample_app - CMakeLists.txt

if(CONFIG_SPWM_SAMPLE_APP_ENABLED)
    if(${BUILD} STREQUAL "EXAMPLE")
        target_sources(${TARGET} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/spwm_sample_app.c
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