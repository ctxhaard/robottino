#!/bin/bash

. setenv

#sudo mount -t nfs -v -o proto=tcp,port=2049 ${TARGET}:/etc /mnt/robottino-etc
sudo mount -t nfs -v -o proto=tcp,port=2049 ${TARGET}:/ /mnt/robottino

