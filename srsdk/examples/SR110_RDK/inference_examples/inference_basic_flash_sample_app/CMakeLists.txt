#SRSDK Sample Applications - Inference Basic Flash Sample App - CMakeLists.txt

if(CONFIG_INFERENCE_BASIC_FLASH_SAMPLE_APP)
    target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/model_flash.cc
    ${CMAKE_CURRENT_LIST_DIR}/model_flash_io.cc
    ${CMAKE_CURRENT_LIST_DIR}/inference_basic_flash_sample_app.cc
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    )

    target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    )
endif()
