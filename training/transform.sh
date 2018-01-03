#!/bin/bash
NAME='map1'
INPUTS=18

INPUTTYPE='road'
DATA_FILE="${NAME}_${INPUTTYPE}.txt"
LINES=`wc -l ${DATA_FILE} | cut -c 1-9`
LINES="$(($LINES / 4))"
INPUTTXT="${LINES} ${INPUTS} 1\n"
echo -e ${INPUTTXT}
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_fw.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_bw.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_right.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_left.txt

python transform.py 0 ${DATA_FILE} >> ${NAME}_${INPUTTYPE}_fw.txt
python transform.py 1 ${DATA_FILE} >> ${NAME}_${INPUTTYPE}_bw.txt
python transform.py 2 ${DATA_FILE} >> ${NAME}_${INPUTTYPE}_right.txt
python transform.py 3 ${DATA_FILE} >> ${NAME}_${INPUTTYPE}_left.txt

