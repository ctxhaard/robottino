#!/bin/bash

source envsetup

while true; do
  echo accepting video streaming
  nc -l $PORT_VID | mplayer -fps 200 -demuxer h264es -
  sleep 2
done


