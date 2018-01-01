#!/bin/bash
NAME='map1'
INPUTS=19

INPUTTYPE='road'

LINES=`wc -l ${NAME}_${INPUTTYPE}.txt | cut -c 1-9`
LINES="$(($LINES / 4 - 500))"
INPUTTXT="${LINES} ${INPUTS} 1\n"
echo -e ${INPUTTXT}
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_fw.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_bw.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_right.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_left.txt

python transform.py 0 >> ${NAME}_${INPUTTYPE}_fw.txt
python transform.py 1 >> ${NAME}_${INPUTTYPE}_bw.txt
python transform.py 2 >> ${NAME}_${INPUTTYPE}_right.txt
python transform.py 3 >> ${NAME}_${INPUTTYPE}_left.txt

INPUTTYPE='offroad'

LINES=`wc -l ${NAME}_${INPUTTYPE}.txt | cut -c 1-9`
LINES="$(($LINES / 4 - 10))"
INPUTTXT="${LINES} ${INPUTS} 1\n"
echo -e ${INPUTTXT}
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_fw.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_bw.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_right.txt
echo -e "${INPUTTXT}" > ${NAME}_${INPUTTYPE}_left.txt

python transform.py 0 >> ${NAME}_${INPUTTYPE}_fw.txt
python transform.py 1 >> ${NAME}_${INPUTTYPE}_bw.txt
python transform.py 2 >> ${NAME}_${INPUTTYPE}_right.txt
python transform.py 3 >> ${NAME}_${INPUTTYPE}_left.txt
