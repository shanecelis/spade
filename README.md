# Spade - an implementation of Sprig engine

This repo is a C implementation of the engine you can find in the "engine" folder of the repo github.com/hackclub/sprig

It was reimplemented in C to run on the Raspberry Pi Pico.

However, debugging is hard when compiling directly to arm-eabi-none, so the engine can also be compiled to x86, in which case it renders to a minifb window.

The environment variable -DSPADE_TARGET can be passed to Cmake to specify which version you would like to build, one you can run on your PC, or one you can run on your Sprig hardware.

### initialization

```
git clone https://github.com/hackclub/spade
cd spade
git submodule update --init --recursive
```

### building for RPI
```
mkdir rpi_build
cd rpi_build
cmake .. -DSPADE_TARGET=rpi
make
cp spade.uf2 /Volumes/RPI-RP2
```

### building for x86 (tested with mac)
```
mkdir pc_build
cd pc_build
cmake .. -DSPADE_TARGET=pc
make
./spade
```
