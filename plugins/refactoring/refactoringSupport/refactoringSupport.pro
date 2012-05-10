QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = refactoringSupport_ru.ts

HEADERS = \
	refactoringPlugin.h \
	refactoringWindow.h \
	refactoringPreferencePage.h \
	refactoringFinder.h \
	refactoringApplier.h \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h \


SOURCES = \
	refactoringPlugin.cpp \
	refactoringWindow.cpp \
	refactoringPreferencePage.cpp \
	refactoringFinder.cpp \
	refactoringApplier.cpp \


FORMS += \
	refactoringWindow.ui \
	refactoringPreferencePage.ui \


#RESOURCES += \
#	refactoring.qrc

RESOURCES += \
    refactoring.qrc
