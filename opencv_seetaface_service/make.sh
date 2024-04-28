#!/bin/sh
make distclean
/opt/Qt5.12.8/5.12.8/gcc_64/bin/qmake
make -j16
