#SRSDK Sample Applications - Inference Multi Model Sample App - CMakeLists.txt

if(CONFIG_INFERENCE_MULTI_MODEL_SAMPLE_APP)
    target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ethosu/model2.cc
    ${CMAKE_CURRENT_LIST_DIR}/ethosu/model2_io.cc
    ${CMAKE_CURRENT_LIST_DIR}/ethosu/model1.cc
    ${CMAKE_CURRENT_LIST_DIR}/ethosu/model1_io.cc
    ${CMAKE_CURRENT_LIST_DIR}/inference_multi_model_sample_app.cc
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    )

    target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    )
endif()
