add_compile_definitions(PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

add_executable(microphone 
        mic.cpp
        ../../src/SPI.c
        ../../src/ST7789.c
        ../../src/graphics.c
        ../../src/I2C.c
        ../../src/io_expander.c
        ../../src/input.c
        ../../src/PWM.c
        ../../src/audio_fifo.c
        ../../src/audio.c
        ../../src/analog.c
        ../../src/LSM6DSL.cpp
        ../../src/MeloperoSensei.cpp
        ../../src/microphone.c
        ../../src/mic-lib-for-pico/pdm_microphone.c
        ../../src/mic-lib-for-pico/OpenPDM2PCM/OpenPDMFilter.c
)

pico_generate_pio_header(microphone ${CMAKE_CURRENT_LIST_DIR}/../../src/mic-lib-for-pico/pdm_microphone.pio)

# pull in common dependencies
target_link_libraries(microphone
        pico_stdlib
        hardware_spi
        hardware_i2c
        hardware_pwm
        hardware_adc
        hardware_pio
        hardware_dma
        hardware_clocks
        pico_rand
)

 # create map/bin/hex file etc.
pico_add_extra_outputs(microphone)