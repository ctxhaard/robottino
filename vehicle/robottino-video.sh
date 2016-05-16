#!/bin/bash

PORT_VID=1235
HOST=192.168.1.121
while true; do
  echo connecting to video server
  raspivid -v -t 0 -w 300 -hf --ISO 800 -w 400 -h 300 -vf -fps 20 -o - | nc $HOST $PORT_VID
  echo failed
  sleep 2
done

