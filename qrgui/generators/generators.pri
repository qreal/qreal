# XMI
HEADERS += generators/xmi/xmiHandler.h \

SOURCES += generators/xmi/xmiHandler.cpp \

# JAVA
HEADERS += generators/java/javaHandler.h \

SOURCES += generators/java/javaHandler.cpp \

# Генератор хаскола
HEADERS += generators/hascol/hascolGenerator.h \

SOURCES += generators/hascol/hascolGenerator.cpp \

# nxtOSEK sequential generator
HEADERS += generators/nxtOSEK/nxtOSEKgenerator.h \
		generators/nxtOSEK/nxtOSEKsequentialGenerator.h \
		generators/nxtOSEK/smartLine.h
SOURCES += generators/nxtOSEK/nxtOSEKsequentialGenerator/nxtOSEKsequentialGenerator.cpp \
		generators/nxtOSEK/private/smartLine.cpp

# nxtOSEK function oriented generator
include (generators/nxtOSEK/nxtOSEKfuncOrientedGenerator.pri)

HEADERS += generators/editorGenerator/editorGenerator.h
SOURCES += generators/editorGenerator/editorGenerator.cpp

# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)

# include (generators/hascol/hascol.pri)
