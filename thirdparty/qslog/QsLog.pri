INCLUDEPATH += $$PWD

# WARNING: QS_LOG_LINE_NUMBERS and QS_LOG_SEPARATE_THREAD were removed from repo version and made enabled by default
#DEFINES += QS_LOG_DISABLE         # logging code is replaced with a no-op

SOURCES += \
	$$PWD/QsLogDest.cpp \
	$$PWD/QsLog.cpp \
	$$PWD/QsLogDestConsole.cpp \
	$$PWD/QsLogDestFile.cpp \
	$$PWD/QsLogDestFunctor.cpp \

HEADERS += \
	$$PWD/QsLogDest.h \
	$$PWD/QsLog.h \
	$$PWD/QsLogDestConsole.h \
	$$PWD/QsLogLevel.h \
	$$PWD/QsLogDestFile.h \
	$$PWD/QsLogDisableForThisFile.h \
	$$PWD/QsLogDestFunctor.h \

OTHER_FILES += \
	$$PWD/QsLogChanges.txt \
	$$PWD/QsLogReadme.txt \
	$$PWD/LICENSE.txt \
