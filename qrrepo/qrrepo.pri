include(../global.pri)

links(qrkernel qrutils)

HEADERS += \
	$$PWD/private/repository.h \
	$$PWD/private/folderCompressor.h \
	$$PWD/private/qrRepoGlobal.h \
	$$PWD/private/serializer.h \
	$$PWD/private/singleXmlSerializer.h \
	$$PWD/private/valuesSerializer.h \
	$$PWD/private/classes/object.h \
	$$PWD/private/classes/logicalObject.h \
	$$PWD/private/classes/graphicalObject.h \
	$$PWD/private/classes/graphicalPart.h \

SOURCES += \
	$$PWD/private/repository.cpp \
	$$PWD/private/folderCompressor.cpp \
	$$PWD/private/repoApi.cpp \
	$$PWD/private/serializer.cpp \
	$$PWD/private/singleXmlSerializer.cpp \
	$$PWD/private/valuesSerializer.cpp \
	$$PWD/private/classes/object.cpp \
	$$PWD/private/classes/logicalObject.cpp \
	$$PWD/private/classes/graphicalObject.cpp \
	$$PWD/private/classes/graphicalPart.cpp \

# repo API
HEADERS += \
	$$PWD/repoApi.h \
	$$PWD/graphicalRepoApi.h \
	$$PWD/logicalRepoApi.h \
	$$PWD/repoControlInterface.h \
	$$PWD/commonRepoApi.h \

DEFINES += QRREPO_LIBRARY

QT += xml
