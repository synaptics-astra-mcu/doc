# sm CMakeLists.txt

target_sources(${TARGET} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/emmc_boot.c
${CMAKE_CURRENT_LIST_DIR}/process_bootsource.c
${CMAKE_CURRENT_LIST_DIR}/xspi_boot.c
)

target_include_directories(${TARGET} PUBLIC
${CMAKE_CURRENT_LIST_DIR})

