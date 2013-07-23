QREAL_XML = bPMNMetamodel.xml
QREAL_EDITOR_PATH = bpmn
ROOT = ../..

include (../../plugins/editorsSdk/editorsCommon.pri)

win32 {
	QMAKE_POST_LINK = "cmd /C "xcopy help ..\\..\\bin\\help\\ /s /e /q /y /i &&"\
						" xcopy examples ..\\..\\bin\\examples /s /e /q /y /i &&"\
						" xcopy changelog.txt ..\\..\\bin\\""
}
else {
	QMAKE_POST_LINK = "mkdir ../../bin/help/;"\
						" cp -r help/* ../../bin/help/ &&"\
						" mkdir ../../bin/examples/;"\
						" cp -r examples/* ../../bin/examples/;" \
						" cp changelog.txt ../../bin/"
}
