DEFINES += QRKERNEL_LIBRARY

CONFIG += c++11

INCLUDEPATH += \
	$$PWD/..

HEADERS += \
	$$PWD/ids.h \
	$$PWD/definitions.h \
	$$PWD/exception/exception.h \
	$$PWD/roles.h \
	$$PWD/settingsManager.h \
	$$PWD/kernelDeclSpec.h \
	$$PWD/timeMeasurer.h \
	$$PWD/version.h \

SOURCES += \
	$$PWD/ids.cpp \
	$$PWD/exception/exception.cpp \
	$$PWD/settingsManager.cpp \
	$$PWD/timeMeasurer.cpp \
	$$PWD/version.cpp \

RESOURCES += \
	$$PWD/qrkernel.qrc \
