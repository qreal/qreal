TARGET = qrrepo_unittests

include(../common.pri)

links(qslog)

include(../../../qrrepo/qrrepo.pri)

INCLUDEPATH += \
	$$PWD/../../../qrrepo/ \

SOURCES += \
	repoApiTest.cpp \
	privateTests/folderCompressorTest.cpp \
	privateTests/serializerTest.cpp \
	privateTests/repositoryTest.cpp \
	privateTests/classesTests/objectTest.cpp \
	privateTests/classesTests/graphicalObjectTest.cpp \

HEADERS += \
	repoApiTest.h \
	privateTests/folderCompressorTest.h \
	privateTests/serializerTest.h \
	privateTests/repositoryTest.h \
