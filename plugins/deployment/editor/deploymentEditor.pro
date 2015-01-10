QREAL_XML = deploymentEditor.xml
QREAL_XML_DEPENDS =  ../../commonMetamodels/kernelMetamodel.xml ../../commonMetamodels/basicBehaviorsMetamodel.xml ../../commonMetamodels/basicActionsMetamodel.xml
QREAL_EDITOR_PATH = deployment/editor
ROOT = ../../..

include (../../editorsSdk/editorsCommon.pri)

win32 {
	QMAKE_POST_LINK = "cmd /C "xcopy images ..\\..\\..\\bin\\images /s /e /q /y /i""
}
else {
	QMAKE_POST_LINK = "cp -r images ../../../bin/"
}
