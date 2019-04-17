#!/bin/bash

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"

#make the FluxProgBackend if build don't exists
BACK_END_PATH=$SCRIPTPATH$"/../FluxProgBackend/build/bin"
if [ ! -d "$BACK_END_PATH" ]; then
    cd ../FluxProgBackend && mkdir build
    cd build  &&  cmake .. && make
fi

cd "$SCRIPTPATH"
#make the FluxProg if build don't exists
FRONT_END_PATH=$SCRIPTPATH$"/build/bin"
if [ ! -d "$FRONT_END_PATH" ]; then
    mkdir build
    cd build  &&  cmake .. && make
fi

#execute
"$FRONT_END_PATH/FluxProg"
