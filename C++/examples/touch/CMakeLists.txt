add_compile_definitions(PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(melopero_sensei_touch C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    ../../src/audio.c
    ../../src/audio_fifo.c
    ../../src/analog.c
    ../../src/graphics.c
    ../../src/I2C.c
    ../../src/input.c
    ../../src/io_expander.c
    ../../src/MeloperoSensei.cpp
    ../../src/PWM.c
    ../../src/SPI.c
    ../../src/ST7789.c
    ../../src/MPR121.c
    touch_example.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_i2c
    hardware_pwm
    hardware_spi
    hardware_adc
    
)

# Enable usb output, disable uart output
#pico_enable_stdio_usb(${PROJECT_NAME} 1)
#pico_enable_stdio_uart(${PROJECT_NAME} 0)