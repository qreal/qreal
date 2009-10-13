cd ..
qmake

nmake

qmake -tp vc -recursive

cd tools\VisualStudioProjectsFix

qmake

nmake -f Makefile.Release

cd release

VisualStudioProjectsFix

cd ..

cd ..