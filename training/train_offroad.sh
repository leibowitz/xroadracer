#!/bin/bash
if [ $# = 0 ]; then
        exit 1
fi
MAPNAME="$1"
./train ${MAPNAME} offroad fw
./train ${MAPNAME} offroad bw
./train ${MAPNAME} offroad right
./train ${MAPNAME} offroad left
./train ${MAPNAME} offroad fw_right
./train ${MAPNAME} offroad fw_left
./train ${MAPNAME} offroad bw_right
./train ${MAPNAME} offroad bw_left
./train ${MAPNAME} offroad none
