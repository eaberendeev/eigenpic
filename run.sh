#!/bin/bash

WorkDir="Res_epic_simulation"

if [[ $1 == "rebuilt" ]]; then
    cd ./src
    make clean
    echo "Object files removed, code ready to be rebuilt!"
    make
    cd ..
    exit
fi

if [[ $1 == "make" ]]; then
    cd ./src
    make
    cd ..
    exit
fi

if [[ $1 != "run" ]]; then
    exit
fi

if [[ $2 == "clean" ]]; then
    rm -rf $WorkDir
    echo "Work directory removed!"
fi


if ! mkdir $WorkDir; then
    echo "Check for delete WorkDir"
    exit
fi
if mv ./src/epic ./$WorkDir; then
    cp -r ./src ./$WorkDir
    cp -r ./utils ./$WorkDir
    cp *.cfg ./$WorkDir
    cp *.sh ./$WorkDir
    echo "The project is assembled and ready to run!"
    cd ./$WorkDir
    ./epic
fi 
