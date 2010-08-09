rem Build QReal
cd ..\..\unreal\trunk\
qmake
nmake

rem Create Qreal VS solution
qmake -tp vc -recursive

rem Build and run VS fixer
cd ..\..\tools\vs\vsFixer\
qmake
nmake
vsFixer