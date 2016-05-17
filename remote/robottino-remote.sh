#!/bin/bash

source envsetup

./robottino-video &
stty -icanon && nc $TARGET $PORT

