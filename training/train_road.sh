#!/bin/bash
if [ $# = 0 ]; then
        exit 1
fi
MAPNAME="$1"
./train ${MAPNAME} road fw
./train ${MAPNAME} road bw
./train ${MAPNAME} road right
./train ${MAPNAME} road left


