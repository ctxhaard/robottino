#!/bin/bash

source envsetup

#bash ./robottino-video.sh &
stty -icanon && nc $TARGET $PORT

