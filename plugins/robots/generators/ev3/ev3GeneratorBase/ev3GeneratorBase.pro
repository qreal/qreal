QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/
TARGET = robots-ev3-generator-base

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
		-lrobots-generator-base -lrobots-interpreter-base \

DEFINES += ROBOTS_EV3_GENERATOR_BASE_LIBRARY

INCLUDEPATH += \
	$$PWD/include \
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

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3GeneratorBase_ru.ts

HEADERS += \
    include/ev3GeneratorBase/ev3GeneratorPluginBase.h \
    include/ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h \
    include/ev3GeneratorBase/ev3MasterGeneratorBase.h \
    src/ev3GeneratorCustomizer.h \
    src/ev3GeneratorFactory.h

SOURCES += \
    src/ev3GeneratorPluginBase.cpp \
    src/ev3MasterGeneratorBase.cpp \
    src/ev3GeneratorCustomizer.cpp \
    src/ev3GeneratorFactory.cpp
