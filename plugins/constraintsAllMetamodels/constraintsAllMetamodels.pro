TEMPLATE = lib
CONFIG += plugin
DESTDIR = D:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -LD:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsAllMetamodelsPlugin.h \
	D:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/qrgui/pluginManager/editorManagerInterface.h \
	constraintsAllLanguages_1.h \


SOURCES += \
	constraintsAllMetamodelsPlugin.cpp \
	D:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsAllLanguages_1.cpp \

