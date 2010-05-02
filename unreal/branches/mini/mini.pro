TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . antlr 
LIBS += -L. -lantlr3c_dll


# Input
HEADERS += javaLexer.h javaParser.h activityLexer.h activityParser.h
SOURCES += javaLexer.c javaParser.c main.cpp activityLexer.c activityParser.c
