QT += widgets network

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-trik-f-sharp-generator

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
		-lrobots-generator-base -lrobots-trik-generator-base -lrobots-utils \

INCLUDEPATH += \
	$$PWD/../trikGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

win32 {
	QMAKE_POST_LINK = "cmd /C "copy lib\\Trik.Core.dll ..\\..\\..\\..\\..\\bin /y""
}
else {
	QMAKE_POST_LINK = "cp lib/Trik.Core.dll ../../../../../bin"
}

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../../bin/plugins/
}

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/trikFSharpGenerator_ru.ts

HEADERS += \
	$$PWD/trikFSharpGeneratorPlugin.h \
	$$PWD/trikFSharpMasterGenerator.h \

SOURCES += \
	$$PWD/trikFSharpGeneratorPlugin.cpp \
	$$PWD/trikFSharpMasterGenerator.cpp \

RESOURCES = \
	$$PWD/trikFSharpGenerator.qrc \
	$$PWD/templates.qrc \
