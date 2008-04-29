QT		+=  sql network
INCLUDEPATH	+= ../../repo  .
unix:INCLUDEPATH += /opt/Ice-3.2/include
LIBS += -lIce -lIceUtil
unix:LIBS += -L/opt/Ice-3.2/lib/
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
