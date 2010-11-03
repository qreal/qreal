# XMI
HEADERS += generators/xmi/xmiHandler.h \

SOURCES += generators/xmi/xmiHandler.cpp \

# JAVA
HEADERS += generators/java/javaHandler.h \

SOURCES += generators/java/javaHandler.cpp \

# Генератор хаскола
HEADERS += generators/hascol/hascolGenerator.h \

SOURCES += generators/hascol/hascolGenerator.cpp \


# Генератор метаредактора
HEADERS += generators/editorGenerator/editorGenerator.h \

SOURCES += generators/editorGenerator/editorGenerator.cpp \

# Генератор машин состояний для проекта viTv
HEADERS += generators/viTv/viTvGenerator.h \

SOURCES += generators/viTv/viTvGenerator.cpp \


# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)

# include (generators/hascol/hascol.pri)
