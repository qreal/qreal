CONFIG += no_keywords
INCLUDEPATH += . generators/java/antlr

# XMI
HEADERS += generators/xmi/xmiHandler.h \

SOURCES += generators/xmi/xmiHandler.cpp \

# JAVA
HEADERS += generators/java/javaHandler.h \
			generators/java/tree.h \
			generators/java/javaLexer.h \
			generators/java/javaParser.h \
			generators/java/structure.h \
			generators/java/method.h \
			generators/java/attribute.h \

SOURCES += generators/java/javaHandler.cpp \
			generators/java/tree.cpp \
			generators/java/javaLexer.c \
			generators/java/javaParser.c \
			generators/java/structure.cpp \
			generators/java/method.cpp \
			generators/java/attribute.cpp \

# Генератор редакторов
HEADERS += generators/editors/editorGenerator.h \

SOURCES += generators/editors/editorGenerator.cpp \

# Генератор хаскола
HEADERS += generators/hascol/hascolGenerator.h \

SOURCES += generators/hascol/hascolGenerator.cpp \

# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)

# include (generators/hascol/hascol.pri)
