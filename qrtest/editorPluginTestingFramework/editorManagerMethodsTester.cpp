#include "editorManagerMethodsTester.h"

#include <QtCore/QDebug>

using namespace editorPluginTestingFramework;
using namespace qReal;

EditorManagerMethodsTester::EditorManagerMethodsTester(EditorManager* qrxcGeneratedPlugin
		, InterpreterEditorManager* interpreterGeneratedPlugin)
{
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
	mInterpreterGeneratedPlugin = interpreterGeneratedPlugin;
}

class EditorManagerMethodsTester::StringGenerator
{
public:
	QStringList convertIdListIntoStringList(IdList idList) const
	{
		QStringList resultList;

		foreach (Id const &id, idList) {
			resultList += id.toString();
		}

		return resultList;
	}

	virtual QString generateString(EditorManagerInterface *editorManagerInterface) const {
		QString resultStr = "";

		foreach (QString const &elementOfList, generateList(editorManagerInterface)) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}

	virtual QString methodName() const = 0;

protected:

	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const = 0;

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &editorId = Id::rootId()
			, Id const &diagramId = Id::rootId()
			, Id const &elementId = Id::rootId()) const = 0;
};

class EditorManagerMethodsTester::StringGeneratorForEditors : public EditorManagerMethodsTester::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			QStringList const additionalList = callMethod(editorManagerInterface, editor);
			resultList += additionalList;
			resultList += "|";
		}
		return resultList;
	}
};

class EditorManagerMethodsTester::StringGeneratorForDiagrams : public EditorManagerMethodsTester::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				QStringList const additionalList = callMethod(editorManagerInterface, editor, diagram);
				resultList += additionalList;
				resultList += "|";
			}
		}
		return resultList;
	}
};

class EditorManagerMethodsTester::StringGeneratorForElements : public EditorManagerMethodsTester::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				foreach (Id const &element, editorManagerInterface->elements(diagram)) {
					QStringList const additionalList = callMethod(editorManagerInterface, editor, diagram, element);
					resultList += additionalList;
					resultList += "|";
				}
			}
		}
		return resultList;
	}
};

class EditorManagerMethodsTester::DiagramsListGenerator : public EditorManagerMethodsTester::StringGeneratorForEditors
{
	virtual QString methodName() const {
		return "Diagrams";
	}

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		return convertIdListIntoStringList(editorManagerInterface->diagrams(editorId));
	}
};

class EditorManagerMethodsTester::ElementsListGeneratorWithIdParameter : public EditorManagerMethodsTester::StringGeneratorForDiagrams
{
	virtual QString methodName() const {
		return "Elements(Id const &diagram)";
	}

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		return convertIdListIntoStringList(editorManagerInterface->elements(diagramId));
	}
};

class EditorManagerMethodsTester::ElementsListGeneratorWithQStringParameters : public EditorManagerMethodsTester::StringGeneratorForDiagrams
{
	virtual QString methodName() const {
		return "Elements(QString const &editor, QString const &diagram)";
	}

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId) const
	{
		Q_UNUSED(elementId);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();
		return (editorManagerInterface->elements(editorName, diagramName));
	}
};

void EditorManagerMethodsTester::testMethods()
{
	testMethod(DiagramsListGenerator());

	testMethod(ElementsListGeneratorWithIdParameter());
	testMethod(ElementsListGeneratorWithQStringParameters());
}

void EditorManagerMethodsTester::testMethod(StringGenerator const &stringGenerator)
{
	QString const &methodName = stringGenerator.methodName();

	QString const &qrxcResult = stringGenerator.generateString(mQrxcGeneratedPlugin);
	QString const &interpreterResult = stringGenerator.generateString(mInterpreterGeneratedPlugin);

	QPair<QString, QString> methodsPair = qMakePair(qrxcResult, interpreterResult);
	QPair<QString, QPair<QString, QString> > resultPair = qMakePair(methodName, methodsPair);

	mGeneratedList.append(resultPair);
}

QList<QPair<QString, QPair<QString, QString> > > EditorManagerMethodsTester::generatedResult()
{
	return mGeneratedList;
}
