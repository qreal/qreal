QREAL_XML = ../commonXml/kernel_metamodel.xml
QREAL_EDITOR_NAME = kernelEditor

include (../editorsCommon.pri)

HEADERS += \
	../../qrgui/pluginInterface/listenerInterface.h \
	../../qrgui/pluginInterface/listenerApiInterface.h

HEADERS += \
	contextMenuActionTrigger.h \
	methodeditordialog.h \
	fieldeditordialog.h \

SOURCES += \
	../../qrgui/kernel/ids.cpp \
	contextMenuActionTrigger.cpp \
	methodeditordialog.cpp \
	fieldeditordialog.cpp \

FORMS += \
	methodeditordialog.ui \
	fieldeditordialog.ui \

