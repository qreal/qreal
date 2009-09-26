# XMI
HEADERS += generators/xmi/xmiHandler.h \

SOURCES += generators/xmi/xmiHandler.cpp \

# JAVA
HEADERS += generators/java/javaHandler.h \

SOURCES += generators/java/javaHandler.cpp \

# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)
