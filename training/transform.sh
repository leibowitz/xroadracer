#!/bin/bash
LINES=`wc -l map1.txt | cut -c 1-9`
LINES="$(($LINES / 4))"
#echo $LINES
INPUT="${LINES} 19 1\n"
echo -e $INPUT
echo -e "${INPUT}" > map1_fw.txt
echo -e "${INPUT}" > map1_bw.txt
echo -e "${INPUT}" > map1_right.txt
echo -e "${INPUT}" > map1_left.txt
#exit
python transform.py 0 >> map1_fw.txt
python transform.py 1 >> map1_bw.txt
python transform.py 2 >> map1_right.txt
python transform.py 3 >> map1_left.txt
