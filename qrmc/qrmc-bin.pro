include(../global.pri)
include (classes/classes.pri)
include (utils/utils.pri)


HEADERS += \
	metaCompiler.h \
	../qrkernel/definitions.h \
	../qrkernel/ids.h \
	../qrrepo/repoApi.h \
	diagram.h \
	editor.h \

SOURCES += \
	metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \
	main.cpp \

QT += xml

CONFIG += c++11

CONFIG += console
TARGET = qrmc
                 
links(qrutils) 
links(qrrepo)
links(qrkernel)