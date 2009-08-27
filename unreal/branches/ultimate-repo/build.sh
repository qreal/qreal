#!/bin/sh
#./clean.sh && 
./generate-code && qmake && make -j2 && cd reposerver && qmake && make -j2
