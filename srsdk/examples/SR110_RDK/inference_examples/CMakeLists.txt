#SRSDK Sample Applications - Inference CMakeLists.txt

if(CONFIG_NPU_SAMPLE_APP_ENABLED)
    if((${BUILD} STREQUAL "EXAMPLE"))
        add_subdirectory(inference_basic_flash_sample_app)

        add_subdirectory(inference_basic_sample_app)

        add_subdirectory(inference_multi_model_sample_app)

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
