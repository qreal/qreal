QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-trik-runtime-uploader-plugin

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
		-lrobots-generator-base -lrobots-trik-generator-base \

INCLUDEPATH += \
	$$PWD/../trikGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../../bin/plugins/
}

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikRuntimeUploaderPlugin_ru.ts

HEADERS += \
	$$PWD/trikRuntimeUploaderPlugin.h \

SOURCES += \
	$$PWD/trikRuntimeUploaderPlugin.cpp \

RESOURCES = \
	$$PWD/trikRuntimeUploaderPlugin.qrc \
