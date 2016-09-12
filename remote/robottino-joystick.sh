#!/bin/bash

source envsetup

#bash ./robottino-video.sh &
./joystick | nc  $TARGET $PORT

