# XMI
HEADERS += generators/xmi/xmiHandler.h \

SOURCES += generators/xmi/xmiHandler.cpp \

# JAVA
HEADERS += generators/java/javaHandler.h \

SOURCES += generators/java/javaHandler.cpp \

# Генератор редакторов
HEADERS += generators/editors/editorGenerator.h \

SOURCES += generators/editors/editorGenerator.cpp \

# Генератор хаскола
HEADERS += generators/hascol/hascolGenerator.h \

SOURCES += generators/hascol/hascolGenerator.cpp \



HEADERS += generators/metaGenerator/metaGenerator.h \

SOURCES += generators/metaGenerator/metaGenerator.cpp \



# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)

# include (generators/hascol/hascol.pri)
