#!/bin/bash

source envsetup

#bash ./robottino-video.sh &
stty -icanon && nc -u $TARGET $PORT

