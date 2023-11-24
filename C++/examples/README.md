# Examples

## Prerequisites

To build and run the examples you will need to have the Pico SDK set up.

## Build

1. Enter the example directory
2. Create a build folder within the examplen directory. Here all the build files will be generated.
3. Enter the build directory.
4. Run `cmake ..`
5. Run `make`

To sum it up:

```bash
cd examples/<example>
mkdir build
cd build
cmake ..
make
```

## Run

After the build there will be a `<example>.uf2` file in the build folder. Copy this file to your sensei board. The sensei board should restart and run the code :party:.