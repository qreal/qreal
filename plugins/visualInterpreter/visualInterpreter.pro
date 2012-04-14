QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../bin -lqrkernel -lqrutils

TRANSLATIONS = visualInterpreter_ru.ts

HEADERS = \
	../../qrgui/dialogs/preferencesPages/preferencesPage.h \
	visualInterpreterPlugin.h \
	visualInterpreterPreferencesPage.h \
	graphTransformationUnit.h \
	ruleParser.h \

SOURCES = \
	visualInterpreterPlugin.cpp \
	visualInterpreterPreferencesPage.cpp \
	graphTranformationUnit.cpp \
	ruleParser.cpp \

FORMS += \
	visualInterpreterPreferencePage.ui \

RESOURCES += \
	visualInterpreter.qrc \
