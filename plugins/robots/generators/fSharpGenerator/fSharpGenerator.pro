QT += widgets network
CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../../bin/plugins/kitPlugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

win32 {
	QMAKE_POST_LINK = "cmd /C "copy lib\\Trik.Core.dll ..\\..\\..\\..\\bin /y""
}
else {
	QMAKE_POST_LINK = "cp lib/Trik.Core.dll ../../../../bin"
}

LIBS += -L../../../../bin -lqrkernel -lqrutils -lqrrepo -lgeneratorBase -lutils

INCLUDEPATH += \
	$$PWD/../generatorBase/include/ \
	$$PWD/../../interpreters/interpreterBase/include \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
QMAKE_POST_LINKQMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../bin/plugins/
}

TRANSLATIONS = fSharpGenerator_ru.ts

RESOURCES = \
	$$PWD/fSharpGenerator.qrc \
	$$PWD/templates.qrc \

HEADERS += \
	$$PWD/fSharpGeneratorPlugin.h \
	$$PWD/fSharpMasterGenerator.h \
	$$PWD/fSharpGeneratorCustomizer.h \
	$$PWD/fSharpGeneratorFactory.h \
	$$PWD/parts/fSharpVariables.h \
	$$PWD/converters/backgroundColorConverter.h \
	$$PWD/converters/ledColorConverter.h \
	$$PWD/converters/engineV4PortConverter.h \
	$$PWD/converters/engineV6PortConverter.h \
	$$PWD/converters/encoderV4PortConverter.h \
	$$PWD/converters/encoderV6PortConverter.h \
	$$PWD/converters/fSharpStringPropertyConverter.h \
	$$PWD/simpleGenerators/fSharpEnginesGenerator.h \
	$$PWD/simpleGenerators/fSharpEnginesStopGenerator.h \
	$$PWD/simpleGenerators/fSharpNullificationEncoderGenerator.h \
	$$PWD/simpleGenerators/waitForInfraredSensorGenerator.h \
	$$PWD/simpleGenerators/waitForMotionGenerator.h \

SOURCES += \
	$$PWD/fSharpGeneratorPlugin.cpp \
	$$PWD/fSharpMasterGenerator.cpp \
	$$PWD/fSharpGeneratorCustomizer.cpp \
	$$PWD/fSharpGeneratorFactory.cpp \
	$$PWD/parts/fSharpVariables.cpp \
	$$PWD/converters/backgroundColorConverter.cpp \
	$$PWD/converters/ledColorConverter.cpp \
	$$PWD/converters/engineV4PortConverter.cpp \
	$$PWD/converters/engineV6PortConverter.cpp \
	$$PWD/converters/encoderV4PortConverter.cpp \
	$$PWD/converters/encoderV6PortConverter.cpp \
	$$PWD/converters/fSharpStringPropertyConverter.cpp \
	$$PWD/simpleGenerators/fSharpEnginesGenerator.cpp \
	$$PWD/simpleGenerators/fSharpEnginesStopGenerator.cpp \
	$$PWD/simpleGenerators/fSharpNullificationEncoderGenerator.cpp \
	$$PWD/simpleGenerators/waitForInfraredSensorGenerator.cpp \
	$$PWD/simpleGenerators/waitForMotionGenerator.cpp \

