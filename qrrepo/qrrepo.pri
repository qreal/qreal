HEADERS += \
	$$PWD/private/client.h \
	$$PWD/private/folderCompressor.h \
	$$PWD/private/qrRepoGlobal.h \
	$$PWD/private/serializer.h \
	$$PWD/private/classes/object.h \
	$$PWD/private/classes/logicalObject.h \
	$$PWD/private/classes/graphicalObject.h \
	$$PWD/private/classes/graphicalProperty.h \

SOURCES += \
	$$PWD/private/client.cpp \
	$$PWD/private/folderCompressor.cpp \
	$$PWD/private/repoApi.cpp \
	$$PWD/private/serializer.cpp \
	$$PWD/private/classes/object.cpp \
	$$PWD/private/classes/logicalObject.cpp \
	$$PWD/private/classes/graphicalObject.cpp \
	$$PWD/private/classes/graphicalProperty.cpp \

# repo API
HEADERS += \
	$$PWD/repoApi.h \
	$$PWD/graphicalRepoApi.h \
	$$PWD/logicalRepoApi.h \
	$$PWD/repoControlInterface.h \
	$$PWD/commonRepoApi.h \
