QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = refactoringSupport_ru.ts

HEADERS = \
	refactoringPlugin.h \
	refactoringWindow.h \
	refactoringPreferencePage.h \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h \


SOURCES = \
	refactoringPlugin.cpp \
	refactoringWindow.cpp \
	refactoringPreferencePage.cpp \


FORMS += \
	refactoringWindow.ui \
	refactoringPreferencePage.ui \


#RESOURCES += \
#	refactoring.qrc

RESOURCES += \
    refactoring.qrc
