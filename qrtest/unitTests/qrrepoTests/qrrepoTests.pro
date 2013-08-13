TARGET = qrrepo_unittests

include(../common.pri)

include(../../../qrrepo/qrrepo.pri)

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
