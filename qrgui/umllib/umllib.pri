HEADERS += umllib/edgeElement.h \
	umllib/element.h \
	umllib/nodeElement.h \
	umllib/sdfRenderer.h \
	umllib/elementTitle.h \
	umllib/contextMenuAction.h \
	umllib/sceneGridHandler.h \
	umllib/umlPortHandler.h \
	umllib/embedded/linkers/embeddedLinker.h \
	umllib/embedded/linkers/embeddedLinkers.h \
	umllib/serializationData.h \
	umllib/layout/abstractLayoutHelper.h \
	umllib/layout/graph.h \
	umllib/layout/gvizNeatoLayoutHelper.h \
	umllib/layout/gvizDotLayoutHelper.h \

SOURCES += umllib/edgeElement.cpp \
	umllib/element.cpp \
	umllib/nodeElement.cpp \
	umllib/sdfRenderer.cpp \
	umllib/elementTitle.cpp \
	umllib/contextMenuAction.cpp \
	umllib/sceneGridHandler.cpp \
	umllib/umlPortHandler.cpp \
	umllib/embedded/linkers/embeddedLinker.cpp \
	umllib/embedded/linkers/embeddedLinkers.cpp \
	umllib/serializationData.cpp \
	umllib/layout/graph.cpp \
	umllib/layout/gvizNeatoLayoutHelper.cpp \
	umllib/layout/gvizDotLayoutHelper.cpp \

FORMS += \
	umllib/layout/gvizDotLayoutHelperSettings.ui \
	umllib/layout/gvizNeatoLayoutHelperSettings.ui \
