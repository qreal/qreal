QT		+=  sql network
INCLUDEPATH	+= ../repo 
SOURCES		= main.cpp

OBJECTS_DIR 	= obj
MOC_DIR		= moc

# Model
HEADERS         += \
		../common/realrepoapiclasses.h \
		../common/classes.h \
		../repo/realrepoclient.h

SOURCES	       += \
		../common/realrepoapiclasses.cpp \
		../common/classes.cpp \
		../repo/realrepoclient.cpp
