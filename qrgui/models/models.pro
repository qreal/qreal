TEMPLATE = lib

DESTDIR = $$PWD/../../bin

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS += \
	$$PWD/details/graphicalModel.h \
	$$PWD/details/graphicalPartModel.h \
	$$PWD/details/graphicalModelView.h \
	$$PWD/details/logicalModel.h \
	$$PWD/details/logicalModelView.h \
	$$PWD/details/modelsImplementation/graphicalModelItem.h \
	$$PWD/details/modelsImplementation/abstractModel.h \
	$$PWD/details/modelsImplementation/abstractModelItem.h \
	$$PWD/details/modelsImplementation/graphicalPartModelItem.h \
	$$PWD/details/modelsImplementation/logicalModelItem.h \
	$$PWD/details/modelsImplementation/abstractView.h \
	$$PWD/details/modelsAssistApi.h \
	$$PWD/models.h \
	$$PWD/graphicalModelAssistApi.h \
	$$PWD/logicalModelAssistApi.h \
	$$PWD/details/modelsImplementation/modelIndexesInterface.h \

SOURCES += \
	$$PWD/details/graphicalModel.cpp \
	$$PWD/details/graphicalPartModel.cpp \
	$$PWD/details/graphicalModelView.cpp \
	$$PWD/details/logicalModel.cpp \
	$$PWD/details/logicalModelView.cpp \
	$$PWD/details/modelsImplementation/graphicalModelItem.cpp \
	$$PWD/details/modelsImplementation/abstractModel.cpp \
	$$PWD/details/modelsImplementation/abstractModelItem.cpp \
	$$PWD/details/modelsImplementation/graphicalPartModelItem.cpp \
	$$PWD/details/modelsImplementation/logicalModelItem.cpp \
	$$PWD/details/modelsImplementation/abstractView.cpp \
	$$PWD/details/modelsAssistApi.cpp \
	$$PWD/models.cpp \
	$$PWD/graphicalModelAssistApi.cpp \
	$$PWD/logicalModelAssistApi.cpp \
