#!/bin/bash

source envsetup

#while true; do
  echo accepting video streaming
  nc -k -l $PORT_VID | mplayer -noconsolecontrols -fps 200 -demuxer h264es -
#  sleep 2
#done


