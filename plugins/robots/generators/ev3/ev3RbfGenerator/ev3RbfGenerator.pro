QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-ev3-rbf-generator

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
		-lrobots-generator-base -lrobots-ev3-generator-base \

INCLUDEPATH += \
	$$PWD/../ev3GeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../../bin/plugins/
}

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3RbfGenerator_ru.ts

HEADERS += \
	ev3RbfGeneratorPlugin.h \
	ev3RbfMasterGenerator.h \

SOURCES += \
	ev3RbfGeneratorPlugin.cpp \
	ev3RbfMasterGenerator.cpp \

RESOURCES += \
	ev3RbfGenerator.qrc \
    templates.qrc

OTHER_FILES +=
