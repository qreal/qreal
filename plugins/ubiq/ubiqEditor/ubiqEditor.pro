QREAL_XML = ubiqMetamodel.xml
QREAL_EDITOR_PATH = ubiq/ubiqEditor
ROOT = ../../..

include (../../editorsSdk/editorsCommon.pri)

win32 {
    QMAKE_POST_LINK = "xcopy images ..\\..\\..\\bin\\images /s /e /q /y /i "
}
else {
    QMAKE_POST_LINK = "mkdir ../../../bin/images/ && cp -r images ../../../bin/images/ "
}
