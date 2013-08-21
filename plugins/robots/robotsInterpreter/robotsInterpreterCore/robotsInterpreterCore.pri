QT += xml widgets

INCLUDEPATH += ../../../../

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrrepo -lqrutils

HEADERS += \
	$$PWD/src/customizer.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotsSettingsPage.h \
	$$PWD/src/kitPluginManager.h \
	$$PWD/src/interpreter.h \
	$$PWD/src/utils/tracer.h \
	$$PWD/src/utils/debugHelper.h \


SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/interpreter.cpp \
	$$PWD/src/kitPluginManager.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsSettingsPage.cpp \
	$$PWD/src/commonRobotModel/commonRobotModel.cpp \
	$$PWD/src/utils/tracer.cpp \
	$$PWD/src/utils/debugHelper.cpp \

FORMS += \
	$$PWD/src/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/robotsInterpreterCore_ru.ts

include($$PWD/qrguiIncludes.pri)

include($$PWD/include/robotsInterpreterCore.pri)

