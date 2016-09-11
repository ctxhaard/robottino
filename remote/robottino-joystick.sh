#!/bin/bash

source envsetup

#bash ./robottino-video.sh &
./joystick | nc -u $TARGET $PORT

