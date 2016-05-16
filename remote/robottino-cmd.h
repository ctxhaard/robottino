#!/bin/bash

source setenv

stty -icanon && nc $TARGET $PORT

