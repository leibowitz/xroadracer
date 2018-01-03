#!/bin/bash
if [ $# = 0 ]; then
        exit 1
fi
MAPNAME="$1"
./train ${MAPNAME} road fw
./train ${MAPNAME} road bw
./train ${MAPNAME} road right
./train ${MAPNAME} road left
./train ${MAPNAME} road fw_right
./train ${MAPNAME} road fw_left
./train ${MAPNAME} road bw_right
./train ${MAPNAME} road bw_left
./train ${MAPNAME} road none
