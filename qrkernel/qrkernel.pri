DEFINES += QRKERNEL_LIBRARY

HEADERS += \
	$$PWD/ids.h \
	$$PWD/definitions.h \
	$$PWD/exception/exception.h \
	$$PWD/roles.h \
	$$PWD/settingsManager.h \
	$$PWD/settingsListener.h \
	$$PWD/kernelDeclSpec.h \
	$$PWD/timeMeasurer.h \
	$$PWD/version.h \
	$$PWD/logging.h \
	$$PWD/private/listeners.h \

SOURCES += \
	$$PWD/ids.cpp \
	$$PWD/exception/exception.cpp \
	$$PWD/settingsManager.cpp \
	$$PWD/settingsListener.cpp \
	$$PWD/timeMeasurer.cpp \
	$$PWD/version.cpp \
	$$PWD/logging.cpp \

RESOURCES += \
	$$PWD/qrkernel.qrc \

includes(thirdparty/qslog)
links(qslog)
