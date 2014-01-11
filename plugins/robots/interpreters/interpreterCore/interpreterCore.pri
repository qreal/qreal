QT += xml widgets

INCLUDEPATH += \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrrepo -lqrutils -linterpreterBase

HEADERS += \
	$$PWD/src/customizer.h \
	$$PWD/src/kitPluginManager.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotsSettingsPage.h \
	$$PWD/src/interpreter/interpreter.h \
	$$PWD/src/interpreter/interpreterInterface.h \
	$$PWD/src/interpreter/details/autoconfigurer.h \
	$$PWD/src/interpreter/details/blocksTable.h \
	$$PWD/src/interpreter/details/robotsBlockParser.h \
	$$PWD/src/interpreter/details/thread.h \
	$$PWD/src/utils/tracer.h \
	$$PWD/src/utils/debugHelper.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/kitPluginManager.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsSettingsPage.cpp \
	$$PWD/src/interpreter/interpreter.cpp \
	$$PWD/src/interpreter/details/autoconfigurer.cpp \
	$$PWD/src/interpreter/details/blocksTable.cpp \
	$$PWD/src/interpreter/details/robotsBlockParser.cpp \
	$$PWD/src/interpreter/details/thread.cpp \
	$$PWD/src/utils/tracer.cpp \
	$$PWD/src/utils/debugHelper.cpp \

FORMS += \
	$$PWD/src/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
