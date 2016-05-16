#!/bin/bash

source ./setenv

echo accepting video streaming
nc -l $PORT_VID | mplayer -fps 200 -demuxer h264es -

