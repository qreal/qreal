#include "methodsTesterForQrxcAndInterpreter.h"
#include "convertingMethods.h"
#include "defs.h"
#include "qrkernel/exception/exception.h"
#include <QElapsedTimer>
#include <iostream>
#include <QtCore/QDebug>
#include <cstdint>

using namespace std;

using namespace editorPluginTestingFramework;
using namespace qReal;

MethodsTesterForQrxcAndInterpreter::MethodsTesterForQrxcAndInterpreter(
		EditorManager* qrxcGeneratedPlugin
		, InterpreterEditorManager* interpreterGeneratedPlugin
		)
{
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
	mInterpreterGeneratedPlugin = interpreterGeneratedPlugin;
}



class MethodsTesterForQrxcAndInterpreter::StringGenerator : public AbstractStringGenerator
{
public:
	void init(EditorManagerInterface *editorManagerInterface)
	{
		mEditorManagerInterface = editorManagerInterface;
	}

	virtual QString generateString() const
	{
		QString resultStr = "";

		foreach (QString const &elementOfList, generateList(mEditorManagerInterface)) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}


	virtual QString generateStringTime()
	{
		QString resultStr = "";

		foreach (QString const &elementOfList, generateListTime(mEditorManagerInterface)) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}

	virtual QStringList generateListTime(EditorManagerInterface *editorManagerInterface) = 0;


protected:
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const = 0;


	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId = Id::rootId()
			, Id const &diagramId = Id::rootId()
			, Id const &elementId = Id::rootId()
			, QString const &propertyName = ""
			) const = 0;

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId = Id::rootId()
			, Id const &diagramId = Id::rootId()
			, Id const &elementId = Id::rootId()
			, QString const &propertyName = ""
			) = 0;

private:
	EditorManagerInterface *mEditorManagerInterface;
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			QString const additionalString = ConvertingMethods::transformateOutput(
					callMethod(editorManagerInterface, editor), editor);
			resultList.append(additionalString);
			resultList.append("|");
		}
		return resultList;
	}


	virtual QStringList generateListTime(EditorManagerInterface *editorManagerInterface)
	{
		//QStringList result;
		QStringList res;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			QPair<qint64, int> additional =  callMethodTime(editorManagerInterface, editor);
			res.append(QString::number(additional.first));
		}
		return res;
	}


};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				QString const additionalString = ConvertingMethods::transformateOutput(
						callMethod(editorManagerInterface, editor, diagram), diagram);
				resultList.append(additionalString);
				resultList.append("|");
			}
		}
		return resultList;
	}

	virtual QStringList generateListTime(EditorManagerInterface *editorManagerInterface)
	{
		QStringList result;
		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				QPair<qint64, int> additional = callMethodTime(editorManagerInterface, editor, diagram);
				result.append(QString::number(additional.first));
			}
		}
		return result;
	}

};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				foreach (Id const &element, editorManagerInterface->elements(diagram)) {
					QString const additionalString = ConvertingMethods::transformateOutput(
							callMethod(editorManagerInterface, editor, diagram, element), element);
					resultList.append(additionalString);
					resultList.append("|");
				}
			}
		}
		return resultList;
	}


	virtual QStringList  generateListTime(EditorManagerInterface *editorManagerInterface)
	{
		QStringList result;
		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				foreach (Id const &element, editorManagerInterface->elements(diagram)) {
					QPair<qint64, int> additional =
							callMethodTime(editorManagerInterface, editor, diagram, element);
					result.append(QString::number(additional.first));
				}
			}
		}
		return result;
	}

};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				foreach (Id const &element, editorManagerInterface->elements(diagram)) {
					foreach (QString const &property, editorManagerInterface->propertyNames(element)) {
						QString const additionalString = ConvertingMethods::transformateOutput(
								callMethod(editorManagerInterface, editor, diagram, element, property)
								, Id::rootId(), property + "(" + element.element() + ")");
						resultList.append(additionalString);
						resultList.append("|");
					}
				}
			}
		}
		return resultList;
	}


	virtual QStringList generateListTime(EditorManagerInterface *editorManagerInterface)
	{
		QStringList result;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				foreach (Id const &element, editorManagerInterface->elements(diagram)) {
					foreach (QString const &property, editorManagerInterface->propertyNames(element)) {
						QPair<qint64, int> additional =
								callMethodTime(editorManagerInterface, editor, diagram, element, property);
						result.append(QString::number(additional.first));
					}
				}
			}
		}
		return result;
	}


};

class MethodsTesterForQrxcAndInterpreter::EditorsListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
{
	virtual QString methodName() const
	{
		return "Editors";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		Q_UNUSED(editorManagerInterface);
		return ConvertingMethods::convertIdIntoStringList(editorId);
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		Q_UNUSED(editorManagerInterface);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorId;
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EditorsListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::DiagramsListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
{
	virtual QString methodName() const
	{
		return "Diagrams";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->diagrams(editorId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->diagrams(editorId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;

		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramsListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ElementsListGeneratorWithIdParameter
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Elements(Id const &diagram)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);

		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->elements(diagramId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->elements(diagramId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsListGeneratorWithIdParameter();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ElementsListGeneratorWithQStringParameters
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Elements(QString const &editor, QString const &diagram)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();
		return editorManagerInterface->elements(editorName, diagramName);
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->elements(editorName, diagramName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsListGeneratorWithQStringParameters();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::MouseGesturesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Mouse gestures";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->mouseGesture(elementId));
	}



	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->mouseGesture(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new MouseGesturesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsParentOfGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is Parent Of";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		QStringList result;
		foreach (Id const &parentDiagram, editorManagerInterface->diagrams(editorId)) {
			foreach (Id const &parentElement,
					 editorManagerInterface->elements(diagramId)) {
				bool isParent = editorManagerInterface->isParentOf(elementId, parentElement);
				if (isParent) {
					result << parentElement.toString();
					result <<  " is parent of ";
					result << elementId.toString();
				}
			}
		}
		return result;
	}



	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			foreach (Id const &parentDiagram, editorManagerInterface->diagrams(editorId)) {
				foreach (Id const &parentElement,
						 editorManagerInterface->elements(diagramId)) {
					timer.start();
					bool isParent = editorManagerInterface->isParentOf(elementId, parentElement);
					time += timer.nsecsElapsed();
				}
			}

		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentOfGenerator();
		return clonedGenerator;
	}
};


class MethodsTesterForQrxcAndInterpreter::FriendlyNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Friendly name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->friendlyName(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{

		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->friendlyName(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new FriendlyNameListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::DescriptionListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Description";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->description(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->description(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DescriptionListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PropertyDescriptionListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property description";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		QStringList result;
		try {
			result = ConvertingMethods::convertStringIntoStringList(
					editorManagerInterface->propertyDescription(elementId, propertyName));
		} catch (Exception e) {
			result.append("method failed");
		}
		return result;
	}


	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();

			try {
				editorManagerInterface->propertyDescription(elementId, propertyName);
			} catch (Exception e) {
			}

			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDescriptionListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PropertyDisplayedNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property displayed name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->propertyDisplayedName(elementId,propertyName));
	}


	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->propertyDisplayedName(elementId,propertyName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDisplayedNameListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ContainedTypesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Contained types";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->containedTypes(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->containedTypes(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ContainedTypesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ExplosionsListGenerator
: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Explosions";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertExplosionListIntoStringList(editorManagerInterface->explosions(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->explosions(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ExplosionsListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::EnumValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual QString methodName() const
	{
		return "Enum values";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &name = elementId.element();
		return ConvertingMethods::convertingQPairListIntoStringList(editorManagerInterface->enumValues(elementId, name));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &name = elementId.element();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->enumValues(elementId, name);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EnumValuesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::TypeNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Type name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &name = elementId.element();
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->typeName(elementId, name));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &name = elementId.element();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->typeName(elementId, name);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new TypeNameListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ReferencePropertiesGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Reference Properties";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return (editorManagerInterface->referenceProperties(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->referenceProperties(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ReferencePropertiesGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::AllChildrenTypesOfListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "All children types of";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return (editorManagerInterface->allChildrenTypesOf(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->allChildrenTypesOf(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new AllChildrenTypesOfListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsEditorListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
{
	virtual QString methodName() const
	{
		return "Is editor (for editors)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isEditor(editorId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(elementId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->isEditor(editorId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsEditorListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsDiagramListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const {
		return "Is diagram (for diagrams)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isDiagram(diagramId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->isDiagram(diagramId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsDiagramListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsElementListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is element (for elements)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isElement(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->isElement(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsElementListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PropertyNamesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Property names";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return (editorManagerInterface->propertyNames(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->propertyNames(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyNamesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PortTypesListGenerator
: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Port types";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return editorManagerInterface->portTypes(elementId);
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->portTypes(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PortTypesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::DefaultPropertyValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property default value";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->defaultPropertyValue(elementId, propertyName));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->defaultPropertyValue(elementId, propertyName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DefaultPropertyValuesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PropertiesWithDefaultValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Properties with default values";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return (editorManagerInterface->propertiesWithDefaultValues(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->propertiesWithDefaultValues(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertiesWithDefaultValuesListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::HasElementListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Has element";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->hasElement(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->hasElement(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new HasElementListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::FindElementByTypeListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Find element by type";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &type = elementId.element();
		return ConvertingMethods::convertIdIntoStringList(editorManagerInterface->findElementByType(type));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &type = elementId.element();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->findElementByType(type);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new FindElementByTypeListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsGraphicalElementListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is graphical element";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertBoolIntoStringList(
				editorManagerInterface->isGraphicalElementNode(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->isGraphicalElementNode(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsGraphicalElementListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsNodeOrEdgeListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is node or edge";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &elementName = elementId.element();
		return ConvertingMethods::convertIntIntoStringList(
				editorManagerInterface->isNodeOrEdge(editorName, elementName));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &elementName = elementId.element();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->isNodeOrEdge(editorName, elementName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsNodeOrEdgeListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::DiagramNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->diagramName(editorName, diagramName));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->diagramName(editorName, diagramName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNameListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::DiagramNodeNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram node name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->diagramNodeName(editorName, diagramName));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		QString const &editorName = editorId.editor();
		QString const &diagramName = diagramId.diagram();

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->diagramNodeName(editorName, diagramName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNodeNameListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::IsParentPropertyListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Is parent property";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		return ConvertingMethods::convertBoolIntoStringList(
				editorManagerInterface->isParentProperty(elementId, propertyName));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->isParentProperty(elementId, propertyName);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentPropertyListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ChildrenListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Children";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->children(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->children(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ChildrenListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::ShapeListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Shape";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->shape(elementId));
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->shape(elementId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ShapeListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PaletteGroupsGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Palette Groups";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			) const
	{
		Q_UNUSED(propertyName);
		QStringList ololo = editorManagerInterface->paletteGroups(editorId, diagramId);
		return ololo;
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &propertyName
			)
	{
		Q_UNUSED(propertyName);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->paletteGroups(editorId, diagramId);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupsGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups : public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;
		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				if (!editorManagerInterface->paletteGroups(editor, diagram).isEmpty())
					foreach (QString const &group, editorManagerInterface->paletteGroups(editor, diagram)) {
						QString const additionalString = ConvertingMethods::transformateOutput(
								callMethod(editorManagerInterface, editor, diagram, diagram, group), Id::rootId(), group);
						resultList.append(additionalString);
						resultList.append("|");
					}
				else {
					QString const additionalString = ConvertingMethods::transformateOutput(
							callMethod(editorManagerInterface, editor, diagram), editor, "");
					resultList.append(additionalString);

					QString ololo = "Is Empty";

					resultList.append(additionalString);
					resultList.append("|");
					resultList.append(ololo);
					//return resultList;
				}
			}
		}
		return resultList;

	}

	virtual QStringList generateListTime(EditorManagerInterface *editorManagerInterface) {
		QStringList resultList;

		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
				if (!editorManagerInterface->paletteGroups(editor, diagram).isEmpty())
					foreach (QString const &group, editorManagerInterface->paletteGroups(editor, diagram)) {
						QPair<qint64, int> additional = callMethodTime(editorManagerInterface, editor, diagram, diagram, group);
						resultList.append(QString::number(additional.first));
					}
				else {
//					QString const additionalString = ConvertingMethods::transformateOutput(
//							callMethod(editorManagerInterface, editor, diagram), editor, "");
//					resultList.append(additionalString);

//					QString ololo = "Is Empty";

//					resultList.append(additionalString);
//					resultList.append("|");
//					resultList.append(ololo);
//					//return resultList;
				}
			}
		}
		return resultList;
	}
};

class MethodsTesterForQrxcAndInterpreter::PaletteGroupsListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Palette Groups List";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &group
			) const
	{
		return editorManagerInterface->paletteGroupList(editorId, diagramId, group);
	}

	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &group
			)
	{
		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->paletteGroupList(editorId, diagramId, group);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupsListGenerator();
		return clonedGenerator;
	}
};

class MethodsTesterForQrxcAndInterpreter::PaletteGroupDescriptionGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Palette Groups Description";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &group
			) const
	{
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->paletteGroupDescription(editorId, diagramId, group));
	}


	virtual QPair<qint64, int> callMethodTime(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId
			, Id const &diagramId
			, Id const &elementId
			, QString const &group
			)
	{
		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			editorManagerInterface->paletteGroupDescription(editorId, diagramId, group);
			time += timer.nsecsElapsed();
		}
		time = time / 20;
		QPair<qint64, int> result = qMakePair(time, 20);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupDescriptionGenerator();
		return clonedGenerator;
	}
};

void MethodsTesterForQrxcAndInterpreter::testMethods()
{
	fillMethodsToTestList(pathToQrxcInterpreterMethodsToTest);

	mGeneratedList.append(testMethodIfExistsInList(EditorsListGenerator(), "editors"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramsListGenerator(), "diagrams"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsListGeneratorWithIdParameter(), "elements(Id)"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsListGeneratorWithQStringParameters()
			, "elements(QString, QString)"));

	mGeneratedList.append(testMethodIfExistsInList(MouseGesturesListGenerator(), "mouseGestures"));
	mGeneratedList.append(testMethodIfExistsInList(FriendlyNameListGenerator(), "friendlyName"));
	mGeneratedList.append(testMethodIfExistsInList(DescriptionListGenerator(), "description"));
	// есть ли в интерпретаторе??? mGeneratedList.append(testMethodIfExistsInList(PropertyDescriptionListGenerator(), "propertyDescription"));
	mGeneratedList.append(testMethodIfExistsInList(PropertyDisplayedNameListGenerator(), "propertyDisplayedName"));

	mGeneratedList.append(testMethodIfExistsInList(ContainedTypesListGenerator(), "containedTypes"));
	mGeneratedList.append(testMethodIfExistsInList(ExplosionsListGenerator(), "explosions"));
	mGeneratedList.append(testMethodIfExistsInList(EnumValuesListGenerator(), "enumValues"));
	mGeneratedList.append(testMethodIfExistsInList(TypeNameListGenerator(), "typeName"));
	mGeneratedList.append(testMethodIfExistsInList(AllChildrenTypesOfListGenerator(), "allChildrenTypesOf"));

	mGeneratedList.append(testMethodIfExistsInList(IsEditorListGenerator(), "isEditor"));
	mGeneratedList.append(testMethodIfExistsInList(IsDiagramListGenerator(), "isDiagram"));
	mGeneratedList.append(testMethodIfExistsInList(IsElementListGenerator(), "isElement"));

	mGeneratedList.append(testMethodIfExistsInList(PropertyNamesListGenerator(), "propertyNames"));
	mGeneratedList.append(testMethodIfExistsInList(PortTypesListGenerator(), "portTypes"));
	mGeneratedList.append(testMethodIfExistsInList(DefaultPropertyValuesListGenerator(), "defaultPropertyValues"));
	mGeneratedList.append(testMethodIfExistsInList(PropertiesWithDefaultValuesListGenerator()
			, "propertiesWithDefaultValues"));

	mGeneratedList.append(testMethodIfExistsInList(HasElementListGenerator(), "hasElement"));
	mGeneratedList.append(testMethodIfExistsInList(FindElementByTypeListGenerator(), "findElementByType"));
	mGeneratedList.append(testMethodIfExistsInList(IsGraphicalElementListGenerator(), "isGraphicalElement"));

	mGeneratedList.append(testMethodIfExistsInList(IsNodeOrEdgeListGenerator(), "isNodeOrEdge"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNameListGenerator(), "diagramName"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNodeNameListGenerator(), "diagramNodeName"));
	mGeneratedList.append(testMethodIfExistsInList(IsParentPropertyListGenerator(), "isParentProperty"));
	mGeneratedList.append(testMethodIfExistsInList(ChildrenListGenerator(), "children"));
	mGeneratedList.append(testMethodIfExistsInList(ShapeListGenerator(), "shape"));
	//paletteGroups PaletteGroupsGenerator
	mGeneratedList.append(testMethodIfExistsInList(PaletteGroupsGenerator(), "paletteGroups"));
	mGeneratedList.append(testMethodIfExistsInList(PaletteGroupsListGenerator(), "paletteGroupList"));
	mGeneratedList.append(testMethodIfExistsInList(PaletteGroupDescriptionGenerator(), "paletteGroupDescription"));
	mGeneratedList.append(testMethodIfExistsInList(ReferencePropertiesGenerator(), "referenceProperties"));
	mGeneratedList.append(testMethodIfExistsInList(IsParentOfGenerator(), "isParentOf"));
}

QList<QPair<QString, QPair<QString, QString> > > MethodsTesterForQrxcAndInterpreter::generatedResult()
{
	testMethods();
	return mGeneratedList;
}

AbstractStringGenerator * MethodsTesterForQrxcAndInterpreter::initGeneratorWithFirstInterface(
		AbstractStringGenerator const &generator) const
{
	StringGenerator *clonedGenerator = dynamic_cast<StringGenerator *>(generator.clone());

	clonedGenerator->init(mQrxcGeneratedPlugin);
	return clonedGenerator;
}

AbstractStringGenerator * MethodsTesterForQrxcAndInterpreter::initGeneratorWithSecondInterface(
		AbstractStringGenerator const &generator) const
{
	StringGenerator *clonedGenerator = dynamic_cast<StringGenerator *>(generator.clone());

	clonedGenerator->init(mInterpreterGeneratedPlugin);
	return clonedGenerator;
}
