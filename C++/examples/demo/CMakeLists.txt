add_compile_definitions(PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

add_executable(demo 
        demo.cpp
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
        ../../src/MeloperoSensei.cpp
)

# pull in common dependencies
target_link_libraries(demo
        pico_stdlib
        hardware_adc
        hardware_spi
        hardware_i2c
        hardware_pwm
        hardware_clocks
        pico_rand
)

 # create map/bin/hex file etc.
pico_add_extra_outputs(demo)