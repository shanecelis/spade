#!/usr/bin/env bash

fswatch -o /Volumes/RPI-RP2 | xargs -I {} -n 4 cp rpi_build/spade.uf2 /Volumes/RPI-RP2
