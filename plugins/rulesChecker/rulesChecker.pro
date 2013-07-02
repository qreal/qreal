QT += xml widgets
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../bin -lqrkernel -lqrutils

TRANSLATIONS = robotsInterpreter_ru.ts

HEADERS += \
	rulesChecker.h \
	rulesPlugin.h \

SOURCES += \
	rulesChecker.cpp \
	rulesPlugin.cpp \

RESOURCES += \
	icons/icons.qrc \

