QT += xml

TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../../bin -lqrkernel -lqrutils -lqrrepo

INCLUDEPATH += \
	$$PWD/../../../.. \
	$$PWD/../../../../qrgui/ \

QT += widgets

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/refactoringSupport_ru.ts

HEADERS = \
	refactoringPlugin.h \
	refactoringWindow.h \
	refactoringPreferencePage.h \
	refactoringFinder.h \
	refactoringApplier.h \


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
