######################################################################
# Enumerator
######################################################################


PROJECT = event
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += ../../src
QMAKE_LIBDIR += ../../../../

OBJECTS_DIR    = tmp
MOC_DIR        = tmp
UI_DIR         = tmp

SOURCES += main.cpp PortListener.cpp
HEADERS += PortListener.h

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport

