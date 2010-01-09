SOURCES = main.cpp repoclienticeI.cpp repoclientice.cpp
HEADERS =  ../qrgui/kernel/definitions.h ../qrrepo/repoApi.h repoclienticeI.h repoclientice.h

CONFIG += console warn_on debug
QMAKE_CLEAN += qrice

OBJECTS_DIR     = obj
MOC_DIR         = moc

INCLUDEPATH += ../qrgui

unix:INCLUDEPATH += /opt/Ice-3.3/include
win32:INCLUDEPATH += c:/Ice-3.2.1/include

unix:LIBS += -L/opt/Ice-3.3/lib
win32:LIBS += -L'c:/Ice-3.2.1/lib'

LIBS += -lIce -lIceUtil

# Путь до библиотеки с АПИ. Где-нибудь она найдётся...
LIBS +=  -L../qrgui -lqrrepo

