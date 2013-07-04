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

class EditorManagerMethodsTester::StringGenerator {
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
			, Id const &groupId = Id::rootId()
			, QString const &editorName = ""
			, QString const &diagramName = ""
			, QString const &elementName = ""
			, QString const &groupName = "") const = 0;
};

class EditorManagerMethodsTester::StringGeneratorForEditors : public EditorManagerMethodsTester::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			QStringList const additionalList = callMethod(editorManagerInterface, editor);
			resultList += additionalList;
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
				QString const &diagramName = editorManagerInterface->diagramName(editorManagerInterface->friendlyName(editor)
						, editorManagerInterface->friendlyName(diagram));
				// i have no idea about this bullshit
				QStringList const additionalList = callMethod(editorManagerInterface, editor, diagram, diagramName);
				resultList += additionalList;
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
				QString const &diagramName = editorManagerInterface->diagramName(editorManagerInterface->friendlyName(editor)
						, editorManagerInterface->friendlyName(diagram));
				foreach (QString const &element, editorManagerInterface->elements(diagram)) {
					QStringList const additionalList = callMethod(editorManagerInterface
							, editor
							, diagram
							, diagramName
							, element);
					resultList += additionalList;
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
			, Id const &id
			, QString const &name) const
	{
		Q_UNUSED(name);
		return convertIdListIntoStringList(editorManagerInterface->diagrams(id));
	}
};

class EditorManagerMethodsTester::ElementsListGenerator : public EditorManagerMethodsTester::StringGeneratorForDiagrams
{
	virtual QString methodName() const {
		return "Elements";
	}

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &id
			, QString const &name) const
	{
		Q_UNUSED(name);
		return convertIdListIntoStringList(editorManagerInterface->elements(id));
	}
};

class EditorManagerMethodsTester::FriendlyNameListGenerator : public EditorManagerMethodsTester::StringGeneratorForElements
{
	virtual QString methodName() const {
		return "Friendly name";
	}

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &id
			, QString const &name) const
	{
		Q_UNUSED(name);
		QStringList result;
		result.append(editorManagerInterface->friendlyName(id));
		return result;
	}
};

class EditorManagerMethodsTester::DescriptionListGenerator : public EditorManagerMethodsTester::StringGeneratorForElements
{
	virtual QString methodName() const {
		return "Description";
	}

	virtual QStringList callMethod(EditorManagerInterface *editorManagerInterface
			, Id const &id
			, QString const &name) const
	{
		Q_UNUSED(name);
		QStringList result;
		result.append(editorManagerInterface->description(id));
		return result;
	}
};

void EditorManagerMethodsTester::testMethods()
{
	testMethod(DiagramsListGenerator());
	testMethod(ElementsListGenerator());
	testMethod(FriendlyNameListGenerator());
	testMethod(DescriptionListGenerator());
}

void EditorManagerMethodsTester::testMethod(StringGenerator const &stringGenerator)
{
	//qDebug() << "Testing: " << stringGenerator.methodName();
	QString const &methodName = stringGenerator.methodName();

	QString const &qrxcResult = stringGenerator.generateString(mQrxcGeneratedPlugin);
	//qDebug() << "ololo" << qrxcResult;
	QString const &interpreterResult = stringGenerator.generateString(mInterpreterGeneratedPlugin);

//	if (interpreterResult == qrxcResult) {
//		qDebug() << "Results are the same";
//		if (interpreterResult.isEmpty()) {
//			qDebug() << "THIS METHOD HAS TO BE VERIFIED SOMEHOW!";
//		}
//	} else {
//		qDebug() << "Results are not the same";
//		qDebug() << "For interpreter: " << interpreterResult;
//		qDebug() << "For qrxc: " << qrxcResult;
//	}

//	qDebug() << "\n";

	QPair<QString, QString> methodsPair = qMakePair(qrxcResult, interpreterResult);
	QPair<QString, QPair<QString, QString> > resultPair = qMakePair(methodName, methodsPair);

	mGeneratedList.append(resultPair);
}

QList<QPair<QString, QPair<QString, QString> > > EditorManagerMethodsTester::generatedResult()
{
	return mGeneratedList;
}
