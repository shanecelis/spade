# pico-8 Emulator for Sprig

This repo is derived from sprig's [spade](https://github.com/hackclub/spade) firmware and game engine. However, instead of hosting games using jerryscript, it's hosting pico-8 games using lua with a pico-8 emulator, [pemsa](https://github.com/shanecelis/pemsa).

## Building

Prerequisites:

- The ability to run Bash scripts.
- A C build environment, preferably Clang. On Windows, GCC won't work and you must use Clang. Make sure CMake and Make are both working.

Set up your build environment. All folders need to be in your home directory, although they can be linked if you prefer.

Clone Spade:

```sh
cd ~
git clone -b feature-pico-8 https://github.com/hackclub/spade.git
cd spade
git submodule update --init --recursive
```

Download the Pico SDK:

```sh
mkdir ~/raspberrypi
cd ~/raspberrypi
git clone https://github.com/raspberrypi/pico-sdk.git
git clone https://github.com/raspberrypi/pico-extras.git
cd pico-sdk
git submodule update --init
cd ../pico-extras
git submodule update --init
```

### Pico Build

```sh
cmake --preset=rpi
cd rpi_build
make
```

A UF2 file will be outputted to `spade.uf2`. On macOS, with a Pico plugged in and in BOOTSEL mode (hold down the button while powered on), you can transfer from the CLI with `
cp spade.uf2 /Volumes/RPI-RP2`.

#### Trick: Automatically Copy Firmware When Sprig Connects in BOOTSEL Mode

This will copy the latest firmware to the device when it connects automatically.

``` sh
cd rpi_build
fswatch -o /Volumes/RPI-RP2 | xargs -I {} -n 4 cp spade.uf2 /Volumes/RPI-RP2
```

### PC Build

The PC Build has not been kept up to date. But you can exercise the [pemsa pico-8 emulator](https://github.com/shanecelis/pemsa) using its [SDL example](https://github.com/egordorichev/pemsa-sdl).

## Troubleshooting

If you get an error about a missing Pico SDK, run the following and try again:

```sh
export PICO_SDK_PATH=~/raspberrypi/pico-sdk
export PICO_EXTRAS_PATH=~/raspberrypi/pico-extras
```

### See debug output

Use a program like minicom to see what's coming out of the serial port.

``` sh
minicom -D /dev/cu.usbmodem148444221
```

## TODO

* Get sound working

It's commented out because it sounds horrible and slows down the rendering.

* Add a filesystem

pico-8 has a good means of selecting from multiple games. Games could be uploaded via serial interface as usual. 

* Lua library could not load its math module

Sprig would not work. Unclear why. I would have guessed anything but the math module.
