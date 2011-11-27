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
HEADERS += generators/nxtOSEK/nxtOSEKsequentialGenerator.h \
		generators/nxtOSEK/smartLine.h
SOURCES += generators/nxtOSEK/nxtOSEKsequentialGenerator/nxtOSEKsequentialGenerator.cpp \
		generators/nxtOSEK/private/smartLine.cpp

HEADERS += generators/editorGenerator/editorGenerator.h
SOURCES += generators/editorGenerator/editorGenerator.cpp

# nxtOSEK_FN generator
#include (generators/nxtOSEK_FN/nxtOSEK_FN.pri)

# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)

# include (generators/hascol/hascol.pri)
