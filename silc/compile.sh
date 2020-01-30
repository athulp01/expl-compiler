#!/bin/bash

SILC="/home/athul/Workspace/compiler_lab"
wd=`pwd`

cd $SILC/silc;
./silc $1;

if test $? -eq 0
then
    echo "Compilation success"
    cd $SILC/silc/label;
    ./a.out;
    if test $? -eq 0
    then
        echo "Label translation success"
        echo "Running the program in XSM simulator"
        cd $SILC/xsm_expl;
        ./xsm -e "../silc/translated.xsm";
    else
        echo "Label translation failed"
    fi

else
    echo "Compilation Failed"
fi

cd $wd