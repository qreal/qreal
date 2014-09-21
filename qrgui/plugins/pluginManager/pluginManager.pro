TEMPLATE = lib

DESTDIR = $$PWD/../../../bin

QT += xml

LIBS += -L$$PWD/../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo -ltoolPluginInterface

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS += \
	$$PWD/editorManager.h \
	$$PWD/toolPluginManager.h \
	$$PWD/interpreterEditorManager.h \
	$$PWD/editorManagerInterface.h \
	$$PWD/proxyEditorManager.h \
	$$PWD/pattern.h \
	$$PWD/explosion.h \
	$$PWD/details/patternParser.h \
	$$PWD/details/interpreterElementImpl.h \
	$$PWD/details/interpreterPortImpl.h \

SOURCES += \
	$$PWD/editorManager.cpp \
	$$PWD/toolPluginManager.cpp \
	$$PWD/interpreterEditorManager.cpp \
	$$PWD/proxyEditorManager.cpp \
	$$PWD/pattern.cpp \
	$$PWD/explosion.cpp \
	$$PWD/details/patternParser.cpp \
	$$PWD/details/interpreterElementImpl.cpp \
	$$PWD/details/interpreterPortImpl.cpp \
