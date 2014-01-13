TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = ../../../../bin/plugins/kitPlugins/

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

QT += widgets

INCLUDEPATH += \
	../interpreterBase/include \
	../twoDModel/include \
	../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L../../../../bin -lqrkernel -lutils -linterpreterBase -ltwoDModel

include(src/nxtKitInterpreter.pri)
