QREAL_XML = robotsMetamodel.xml
QREAL_XML_DEPENDS =  ../../../commonMetamodels/kernelMetamodel.xml ../../../commonMetamodels/basicBehaviorsMetamodel.xml ../../../commonMetamodels/basicActionsMetamodel.xml
QREAL_EDITOR_PATH = robots/editor/generated
ROOT = ../../../..

include (../../../editorsSdk/editorsCommon.pri)

win32 {
	QMAKE_POST_LINK = "cmd /C "xcopy ..\\images ..\\..\\..\\..\\bin\\images /s /e /q /y /i &&"\
						" xcopy ..\\\doc\\html ..\\..\\..\\..\\bin\\help\\ /s /e /q /y /i &&"\
						" copy ..\\doc\\changelog.txt ..\\..\\..\\..\\bin /y &&"\
						" copy ..\\..\\interpreters\\interpreterCore\\icons\\splashscreen.png ..\\..\\..\\..\\bin /y &&"\
						" xcopy ..\\..\\examples ..\\..\\..\\..\\bin\\examples /s /e /q /y /i""
}
else {
	QMAKE_POST_LINK = "mkdir ../../../../bin/images/;"\
						" cp -r ../images/* ../../../../bin/images/ &&"\
						" mkdir ../../../../bin/help/;"\
						" cp -r ../doc/html/* ../../../../bin/help/ &&"\
						" cp ../doc/changelog.txt ../../../../bin &&"\
						" cp ../../interpreters/interpreterCore/icons/splashscreen.png ../../../../bin &&"\
						" mkdir ../../../../bin/examples/;"\
						" cp -r ../../examples/* ../../../../bin/examples/"
}
