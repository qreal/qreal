#include <QtCore/QDebug>

#include "methodsTester.h"

MethodsTester::MethodsTester(EditorInterface *qrmcGeneratedPlugin)
{
	mQrmcGeneratedPlugin = qrmcGeneratedPlugin;
}

void MethodsTester::testMethods()
{
	testEditorName();
	testDiagrams();
	testElements();
}

void MethodsTester::testEditorName()
{
	qDebug() << mQrmcGeneratedPlugin->editorName();
}

void MethodsTester::testDiagrams()
{
	foreach (QString const &diagramName, mQrmcGeneratedPlugin->diagrams()) {
		qDebug() << diagramName;
	}
}

void MethodsTester::testElements()
{
	foreach (QString const &diagramName, mQrmcGeneratedPlugin->diagrams()) {
		foreach (QString const &elementName, mQrmcGeneratedPlugin->elements(diagramName)) {
			qDebug() << elementName;
		}
	}
}
