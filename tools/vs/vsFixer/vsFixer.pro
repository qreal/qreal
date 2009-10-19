TEMPLATE = app
DESTDIR = ./
CONFIG += console release

HEADERS += projectTypeFixer.h \
    solutionFileFixer.h \
    fixer.h

SOURCES += main.cpp \
    projectTypeFixer.cpp \
    solutionFileFixer.cpp \