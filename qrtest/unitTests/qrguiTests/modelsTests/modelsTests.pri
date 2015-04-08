include(../../../../qrgui/models/models.pri)

DEFINES += QRGUI_MODELS_LIBRARY

HEADERS += \
	$$PWD/../../mocks/qrgui/models/details/modelsImplementation/modelIndexesInterfaceMock.h \

HEADERS += \
	$$PWD/detailsTests/graphicalPartModelTest.h \

SOURCES += \
	$$PWD/detailsTests/graphicalPartModelTest.cpp \
