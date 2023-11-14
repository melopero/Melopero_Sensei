set(MOD_NAME sensei)
string(TOUPPER ${MOD_NAME} MOD_NAME_UPPER)
add_library(usermod_${MOD_NAME} INTERFACE)

target_sources(usermod_${MOD_NAME} INTERFACE
${CMAKE_CURRENT_LIST_DIR}/${MOD_NAME}.c
${CMAKE_CURRENT_LIST_DIR}/${MOD_NAME}.cpp
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/MeloperoSensei.cpp
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/I2C.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/ST7789.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/io_expander.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/audio.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/graphics.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/input.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/audio_fifo.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/PWM.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/SPI.c
${CMAKE_CURRENT_LIST_DIR}/../../C++/src/analog.c
)

set(MICROPY_C_HEAP_SIZE 10240)

target_include_directories(usermod_${MOD_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../../pico/pico-sdk
)

target_compile_definitions(usermod_${MOD_NAME} INTERFACE
    MODULE_SENSEI_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_${MOD_NAME})

