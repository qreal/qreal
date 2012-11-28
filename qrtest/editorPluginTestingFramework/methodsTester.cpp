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

	QStringList getElements(EditorInterface *editorInterface) const
	{
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &element, editorInterface->elements(diagram)) {
				resultList.append(element);
			}
		}

		return resultList;
	}

	QString getStringFromList(QStringList list) const
	{
		QString resultStr = "";

		foreach (QString const &str, list) {
			resultStr += str + " ";
		}

		return resultStr;
	}
};

class MethodsTester::EditorNameListGenerator : public MethodsTester::ListGenerator {
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		return editorInterface->editorName();
	}

	virtual QString methodName() const {
		QString const &methodName = "Editor Name";
		return methodName;
	}
};

class MethodsTester::DiagramsListGenerator : public MethodsTester::ListGenerator {
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QString resultStr = "";
		foreach (QString const &diagram, editorInterface->diagrams()) {
			resultStr += diagram + "\n";
		}
		return resultStr;
	}

	virtual QString methodName() const {
		QString const &methodName = "Diagrams";
		return methodName;
	}
};

class MethodsTester::ElementsListGenerator : public MethodsTester::ListGenerator {
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QStringList const listOfElements = getElements(editorInterface);
		return getStringFromList(listOfElements);
	}

	virtual QString methodName() const {
		QString const &methodName = "Elements";
		return methodName;
	}
};

class MethodsTester::PropertiesWithDefaultValuesListGenerator : public MethodsTester::ListGenerator
{
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QStringList const listOfElements = getElements(editorInterface);
		QString resultStr = "";

		foreach (QString const &element, listOfElements) {
			QStringList listOfProperties = editorInterface->getPropertiesWithDefaultValues(element);
			resultStr += getStringFromList(listOfProperties) + "\n";
		}

		return resultStr;
	}

	virtual QString methodName() const {
		QString const &methodName = "Properties with default values";
		return methodName;
	}
};

class MethodsTester::TypesContainedByListGenerator : public MethodsTester::ListGenerator
{
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QStringList const listOfElements = getElements(editorInterface);
		QString resultStr = "";

		foreach (QString const &element, listOfElements) {
			QStringList listOfProperties = editorInterface->getTypesContainedBy(element);
			resultStr += getStringFromList(listOfProperties) + "\n";
		}

		return resultStr;
	}

	virtual QString methodName() const {
		QString const &methodName = "Types contained by";
		return methodName;
	}
};

class MethodsTester::ConnectedTypesListGenerator : public MethodsTester::ListGenerator
{
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QStringList const listOfElements = getElements(editorInterface);
		QString resultStr = "";

		foreach (QString const &element, listOfElements) {
			QStringList listOfTypes = editorInterface->getConnectedTypes(element);
			resultStr += getStringFromList(listOfTypes) + "\n";
		}

		return resultStr;
	}

	virtual QString methodName() const {
		QString const &methodName = "Connected types";
		return methodName;
	}
};

class MethodsTester::UsedTypesListGenerator : public MethodsTester::ListGenerator
{
public:
	virtual QString generateList(EditorInterface *editorInterface) const {
		QStringList const listOfElements = getElements(editorInterface);
		QString resultStr = "";

		foreach (QString const &element, listOfElements) {
			QStringList listOfTypes = editorInterface->getUsedTypes(element);
			resultStr += getStringFromList(listOfTypes) + "\n";
		}

		return resultStr;
	}

	virtual QString methodName() const {
		QString const &methodName = "Used types";
		return methodName;
	}
};

void MethodsTester::testMethod(ListGenerator const &listGenerator)
{
	qDebug() << "Testing: " << listGenerator.methodName();

	QString const &qrmcResult = listGenerator.generateList(mQrmcGeneratedPlugin);
	QString const &qrxcResult = listGenerator.generateList(mQrxcGeneratedPlugin);

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
	testMethod(ElementsListGenerator());
	testMethod(PropertiesWithDefaultValuesListGenerator());
	testMethod(ConnectedTypesListGenerator());
	testMethod(UsedTypesListGenerator());
}
