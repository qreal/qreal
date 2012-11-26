#include <QtCore/QDebug>

#include "methodsTester.h"

MethodsTester::MethodsTester(EditorInterface *qrmcGeneratedPlugin, EditorInterface *qrxcGeneratedPlugin)
{
	mQrmcGeneratedPlugin = qrmcGeneratedPlugin;
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
}

class MethodsTester::ListGenerator {
public:
	virtual QString generateList(EditorInterface *editorInterface) const = 0;
	virtual QString methodName() const = 0;
};

class MethodsTester::EditorNameListGenerator : public MethodsTester::ListGenerator {
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		return editorInterface->editorName();
	}

	virtual QString methodName() const {
		QString methodName = "Editor Name";
		return methodName;
	}
};

class MethodsTester::DiagramsListGenerator : public MethodsTester::ListGenerator {
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QString resultStr = "";
		foreach(QString diagram, editorInterface->diagrams()) {
			resultStr += diagram + "\n";
		}
		return resultStr;
	}

	virtual QString methodName() const {
		QString methodName = "Diagrams";
		return methodName;
	}
};

void MethodsTester::testMethod(ListGenerator const &listGenerator) // основная штука, которая берет метод и по нему сравнивает
{
	qDebug() << "Testing: " << listGenerator.methodName();

	QString qrmcResult = listGenerator.generateList(mQrmcGeneratedPlugin);
	QString qrxcResult = listGenerator.generateList(mQrxcGeneratedPlugin);

	if (qrmcResult == qrxcResult) {
		qDebug() << "Method is OK";
	} else {
		qDebug() << "Method is not OK";
		qDebug() << "For qrmc: " << qrmcResult;
		qDebug() << "For qrxc: " << qrxcResult;
	}

	qDebug() << "\n";
}

void MethodsTester::testMethods()
{
	testMethod(EditorNameListGenerator());
	testMethod(DiagramsListGenerator());
}
