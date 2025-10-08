# Build Micropython with "melopero_sensei" Module

## Prerequisites

To build Micropython you will need to install the ARM toolchain and clone this repository in your working directory.

## Build

1. Enter in your working directory 
2. Clone Micropython github repository

`git clone https://github.com/micropython/micropython.git --branch master`

3. Go to in the RP2 ports directory (Raspberry Pi Foundation’s RP2xxx family of microcontrollers)
 
`cd micropython/ports/rp2/`

4. Compile submodules

`make BOARD=RPI_PICO submodules`

5. Compile all (including the "melopero_sensei" external module) 

`make BOARD=RPI_PICO USER_C_MODULES=../../../Melopero_Sensei/MicroPython/src/micropython.cmake`

## Run

After the build there will be a `firmware.uf2` file in the `micropython/ports/rp2/build-RPI_PICO/` directory. Copy this file to your sensei board. The sensei board should restart and run your new custom firmware.
