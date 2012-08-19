QREAL_XML = robotsMetamodel.xml
QREAL_XML_DEPENDS =  ../../../commonMetamodels/kernelMetamodel.xml ../../../commonMetamodels/basicBehaviorsMetamodel.xml ../../../commonMetamodels/basicActionsMetamodel.xml
QREAL_EDITOR_PATH = robots/editor/generated
ROOT = ../../../..

include (../../../editorsSdk/editorsCommon.pri)

win32 {
    QMAKE_POST_LINK = "xcopy ..\\images ..\\..\\..\\..\\bin\\images /s /e /q /y /i && copy ..\\doc\\qreal-robots.qhc ..\\..\\..\\..\\bin /y && copy ..\\doc\\qreal-robots.qch ..\\..\\..\\..\\bin /y && xcopy ..\\..\\examples ..\\..\\..\\..\\bin\\examples /s /e /q /y /i"
}
else {
    QMAKE_POST_LINK = "mkdir ../../../../bin/images/; cp -r ../images/* ../../../../bin/images/ && cp ../doc/qreal-robots.qhc ../../../../bin && cp ../doc/qreal-robots.qch ../../../../bin && mkdir ../../../../bin/examples/; cp -r ../../examples/* ../../../../bin/examples/"
}
