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
	$$PWD/uxInfoInterface.h \

SOURCES += \
	$$PWD/ids.cpp \
	$$PWD/exception/exception.cpp \
	$$PWD/settingsManager.cpp \
	$$PWD/timeMeasurer.cpp \

RESOURCES += \
	$$PWD/qrkernel.qrc
