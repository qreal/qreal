#!/bin/sh
#./clean.sh && 
./generate-code && qmake && make && cd reposerver && qmake && make
