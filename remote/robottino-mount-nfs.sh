#!/bin/bash

source envsetup

sudo mount -t nfs -v -o proto=tcp,port=2049 ${TARGET}:/ /mnt/robottino

