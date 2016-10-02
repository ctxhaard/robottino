#!/bin/bash

source envsetup

#bash ./robottino-video.sh &
./joystick "$1" | nc  $TARGET $PORT

