QT += widgets

INCLUDEPATH += \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel

DEFINES += ROBOTS_INTERPRETER_BASE_LIBRARY

HEADERS += \
	$$PWD/src/blocks/blocksFactory.h \
	$$PWD/src/blocks/details/finalBlock.h \
	$$PWD/src/blocks/details/initialBlock.h \

SOURCES += \
	$$PWD/src/blocks/block.cpp \
	$$PWD/src/blocks/blocksFactory.cpp \
	$$PWD/src/blocks/details/finalBlock.cpp \
	$$PWD/src/blocks/details/initialBlock.cpp \
	$$PWD/src/robotModel/robotModel.cpp \

include($$PWD/include/interpreterBase.pri)
