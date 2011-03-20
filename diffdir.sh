
DIR_A=./kernel
DIR_B=../talkthrough1768/kernel

FILELIST_A=`cd $DIR_A;find . -name "*.[c|h]"`
FILELIST_B=`cd $DIR_B;find . -name "*.[c|h]"`

for FILE in $FILELIST_A
do
    if [ -f "$DIR_A/$FILE" ]
    then
        OUT=`diff "$DIR_A/$FILE" "$DIR_B/$FILE"`
        if [ "$OUT" != "" ]
        then
            echo "===[ $FILE ]========================================"
            diff "$DIR_A/$FILE" "$DIR_B/$FILE"
            echo " "
            echo " "
            echo " "
        fi
    fi
done
