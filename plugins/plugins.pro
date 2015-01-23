TEMPLATE = subdirs

include(common.pri)

# editor plugins
SUBDIRS += \
	$$PWD/dragonDiagram/dragonDiagram.pro \
	$$PWD/blockDiagram/blockDiagram.pro \
#	$$PWD/hascol/hascol.pro \
	$$PWD/metaEditor/metaEditor.pro \
	$$PWD/requirements/requirements.pro \
	$$PWD/domainAnalysis/domainAnalysis.pro \
	$$PWD/deployment/deploymentEditor.pro \

# interpreted plugins
SUBDIRS += \
	generationRulesTool/generationRulesTool.pro \


# tool plugins
SUBDIRS += \
	$$PWD/tools/exterminatus/exterminatus.pro \
	$$PWD/tools/visualInterpreter/visualInterpreter.pro \
	$$PWD/tools/refactoring/refactoring.pro \
#	$$PWD/tools/testInvocation/testInvocation.pro \


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qrutils/release/ -lqrutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qrutils/debug/ -lqrutils
else:unix: LIBS += -L$$OUT_PWD/../qrutils/ -lqrutils

INCLUDEPATH += $$PWD/../qrutils
DEPENDPATH += $$PWD/../qrutils
