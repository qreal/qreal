include (classes/classes.pri)
include (utils/utils.pri)

TARGET = qrmc

include(../global.pri)  

HEADERS += metaCompiler.h \
	  qrkernel/definitions.h \
	  ../qrkernel/ids.h \
	  ../qrrepo/repoApi.h \
	  diagram.h \
	  editor.h \              



SOURCES += metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \

TEMPLATE = lib
QT += xml

CONFIG += c++11

links(qrkernel)    
links(qrrepo)      
