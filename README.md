# pico-8 Emulator for Sprig

This repo is derived from sprig's [spade](https://github.com/hackclub/spade) firmware and game engine. However, instead of hosting games using jerryscript, it's hosting pico-8 games using lua.

## Building

Prerequisites:

- A working Python 3 environment.
- The ability to run Bash scripts.
- A C build environment, preferably Clang. on Windows, GC won't work and you must use Clang. make sure CMake and Make are both working.
- *Optional:* entr installed. Only needed if you want to use jsdev.sh.

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

The PC Build has not been kept up to date. (I may regret this.)

## Troubleshooting

If you get an error about a missing Pico SDK, run the following and try again:

```sh
export PICO_SDK_PATH=~/raspberrypi/pico-sdk
export PICO_EXTRAS_PATH=~/raspberrypi/pico-extras
```
