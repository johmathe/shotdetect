#!/bin/bash
#@author: Peter B.
#@date: 19.Aug.2013

case "$1" in
    gui)
        mkdir -p build
        cd build
        cmake -D CMAKE_BUILD_TYPE=Release .. && make
    ;;
    cmd)
        mkdir -p build
        cd build
        cmake -D CMAKE_BUILD_TYPE=Release -D USE_WXWIDGETS:BOOL=OFF .. && make
    ;;
    all)
        mkdir -p build
        cd build
        cmake .. && make
        cmake -D USE_WXWIDGETS:BOOL=OFF .. && make
    ;;

    *)
        echo ""
        echo " This script just makes it easier to build and compile Shotdetect."
        echo " It just does what Johan wrote down in ../README :) and offers"
        echo " a quick way to build either the commandline- or the GUI-version."
        echo ""
        echo "SYNTAX: $0 (gui|cmd|all)"
        echo ""
        echo ""
esac
