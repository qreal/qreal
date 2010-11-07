# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)
# include (generators/hascol/hascol.pri)
# Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
# include (generators/xmi/xmi.pri)
# include (generators/hascol/hascol.pri)
# Хаскол
HEADERS += parsers/hascol/hascolParser.h \
	parsers/xml/treeElement.h
SOURCES += parsers/hascol/hascolParser.cpp \
	parsers/xml/treeElement.cpp
HEADERS += parsers/xml/xmlParser.h
SOURCES += parsers/xml/xmlParser.cpp # Креатор не умеет показывать файлы глубже одного уровня в иерархии инклудов
