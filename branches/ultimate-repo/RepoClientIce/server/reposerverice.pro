QT		+=  sql network
INCLUDEPATH	+= ../../repo  .
LIBS += -lIce -lIceUtil
SOURCES		= server.cpp repoclienticeI.cpp repoclientice.cpp
QMAKE_CLEAN += reposerverice

OBJECTS_DIR 	= obj
MOC_DIR		= moc

# Model
HEADERS         += \
		../../common/realrepoapiclasses.h \
		../../common/classes.h \
		../../repo/realrepoclient.h \


SOURCES	       += \
		../../common/realrepoapiclasses.cpp \
		../../common/classes.cpp \
		../../repo/realrepoclient.cpp
