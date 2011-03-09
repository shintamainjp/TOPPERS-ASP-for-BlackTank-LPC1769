#!/bin/sh

if [ $# -ne 1 ]
then
    echo "findkeyword.sh [keyword]"
    exit
fi

FILELIST=`find . -name "*.[c|h]"`
KEYWORD=$1

for FILE in $FILELIST
do
    OUT=`grep "$KEYWORD" $FILE`
    if [ ! "$OUT" = "" ]
    then
        echo " "
        echo "[Found in $FILE]"
        grep -i -n "$KEYWORD" $FILE
        echo " "
    fi
done

