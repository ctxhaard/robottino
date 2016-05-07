#!/bin/bash

source envsetup

bash ./robottino-video &
stty -icanon && nc $TARGET $PORT

