#include <QtCore/QDebug>

#include "methodsTester.h"

MethodsTester::MethodsTester(EditorInterface *qrmcGeneratedPlugin, EditorInterface *qrxcGeneratedPlugin)
{
	mQrmcGeneratedPlugin = qrmcGeneratedPlugin;
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
}

void MethodsTester::testMethods()
{
	testEditorName();
	testDiagrams();
	testElements();
}

void MethodsTester::testEditorName()
{
	if (mQrmcGeneratedPlugin->editorName() == mQrxcGeneratedPlugin->editorName()) {
		qDebug() << "editorName: OK";
	} else {
		qDebug() << "editorName: FAILED;";
		qDebug() << "QRMC: " << mQrmcGeneratedPlugin->editorName();
		qDebug() << "QRXC: " << mQrxcGeneratedPlugin->editorName();
	}
}

void MethodsTester::testDiagrams()
{
	QString qrmcDiagrams = "";
	foreach (QString const &diagramName, mQrmcGeneratedPlugin->diagrams()) {
		qrmcDiagrams += diagramName + "\n";
	}

	QString qrxcDiagrams = "";
	foreach (QString const &diagramName, mQrxcGeneratedPlugin->diagrams()) {
		qrxcDiagrams += diagramName + "\n";
	}

	if (qrmcDiagrams == qrxcDiagrams) {
		qDebug() << "diagrams: OK";
	} else {
		qDebug() << "diagrams: FAILED;";
		qDebug() << "QRMC: " << qrmcDiagrams;
		qDebug() << "QRXC: " << qrxcDiagrams;
	}
}

void MethodsTester::testElements()
{
	QString qrmcElements = "";
	foreach (QString const &diagramName, mQrmcGeneratedPlugin->diagrams()) {
		foreach (QString const &elementName, mQrmcGeneratedPlugin->elements(diagramName)) {
			qrmcElements += elementName + "\n";
		}
	}

	QString qrxcElements = "";
	foreach (QString const &diagramName, mQrxcGeneratedPlugin->diagrams()) {
		foreach (QString const &elementName, mQrxcGeneratedPlugin->elements(diagramName)) {
			qrxcElements += elementName + "\n";
		}
	}

	if (qrmcElements == qrxcElements) {
		qDebug() << "elements: OK";
	} else {
		qDebug() << "elements: FAILED;";
		qDebug() << "QRMC: " << qrmcElements;
		qDebug() << "QRXC:" << qrxcElements;
	}
}
