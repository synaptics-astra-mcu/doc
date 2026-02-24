#SRSDK Sample Applications - Inference Basic Sample App CMakeLists.txt

if(CONFIG_INFERENCE_BASIC_SAMPLE_APP)
    target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ethosu/model.cc
    ${CMAKE_CURRENT_LIST_DIR}/ethosu/model_io.cc
    ${CMAKE_CURRENT_LIST_DIR}/inference_basic_sample_app.cc
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    )

    target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    )
endif()
