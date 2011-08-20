#!/bin/sh
# Hack to rebuild the editors see #231
for i in class kernel bpel hascol; do
	cd ${i}Editor
	make distclean && qmake && make
	cd ..
done

