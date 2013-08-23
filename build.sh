#!/bin/bash
#@author: Peter B.
#@date: 19.Aug.2013

case "$1" in
    gui)
        mkdir -p build
        cd build
        cmake ..
    ;;
    cmd)
        mkdir -p build
        cd build
        cmake -D USE_WXWIDGETS:BOOL=OFF ..
    ;;
    all)
        mkdir -p build
        cd build
        cmake ..
        cmake -D USE_WXWIDGETS:BOOL=OFF ..
    ;;

    *)
        echo ""
        echo " This script just makes it easier to build Shotdetect."
        echo " It just does what Johan wrote down in ../README :) and offers"
        echo " a quick way to build either the commandline- or the GUI-version."
        echo ""
        echo "SYNTAX: $0 (gui|cmd|all)"
        echo ""
        echo ""
esac 
