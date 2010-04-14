# XMI
HEADERS += generators/xmi/xmiHandler.h \

SOURCES += generators/xmi/xmiHandler.cpp \

# JAVA
HEADERS += generators/java/javaHandler.h \
			generators/java/tree.h \

SOURCES += generators/java/javaHandler.cpp \
			generators/java/tree.cpp \

# Генератор редакторов
HEADERS += generators/editors/editorGenerator.h \

SOURCES += generators/editors/editorGenerator.cpp \

# Генератор хаскола
HEADERS += generators/hascol/hascolGenerator.h \

SOURCES += generators/hascol/hascolGenerator.cpp \

# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)

# include (generators/hascol/hascol.pri)
