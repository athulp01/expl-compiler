#!/bin/bash

SILC="/home/athul/Workspace/compiler_lab"
wd=`pwd`

cd $SILC/silc;
./silc $1;

cd $SILC/silc/label;
./a.out;

cd $SILC/xsm_expl;
./xsm -e "../silc/translated.xsm";

cd $wd