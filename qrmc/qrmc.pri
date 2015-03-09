include(../global.pri)

QT += xml

DEPENDPATH += \
	. \
	templates \

HEADERS += \
	metaCompiler.h \
	diagram.h \
	editor.h \

SOURCES += \
	metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \
	main.cpp \

include (classes/classes.pri)
include (utils/utils.pri)

links(qrkernel qrutils qrrepo)
includes(qrkernel)