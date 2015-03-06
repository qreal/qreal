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
    templates/elements.h.template \
    templates/pluginInterface.cpp.template \
    templates/pluginInterface.h.template



SOURCES += metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \

TEMPLATE = lib
QT += xml

CONFIG += c++11

links(qrkernel)    
links(qrrepo)      

OTHER_FILES += \
    templates/edge.template \
    templates/node.template \
    templates/plugin.qrc.template \
    templates/plugins.pro.template \
    templates/pro.template \
    templates/utils.template
