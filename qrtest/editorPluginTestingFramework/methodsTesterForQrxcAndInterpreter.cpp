#include "methodsTesterForQrxcAndInterpreter.h"
#include "convertingMethods.h"
#include "defs.h"
#include "qrkernel/exception/exception.h"
#include <QElapsedTimer>
#include <iostream>
#include <QtCore/QDebug>
#include <cstdint>
#include <QtCore/qmath.h>

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

		foreach (QString const &elementOfList, mListOfTime) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}

protected:
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const = 0;

	virtual QPair<qint64, double> dataOfTime() const = 0;

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, Id const &editorId = Id::rootId()
			, Id const &diagramId = Id::rootId()
			, Id const &elementId = Id::rootId()
			, QString const &propertyName = ""
			) const = 0;

	QStringList mutable mListOfTime;

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
			QPair<qint64, double> const additional = dataOfTime();
			mListOfTime.append(QString::number(additional.first));
			mListOfTime.append(" ");
			mListOfTime.append(QString::number(additional.second));
			mListOfTime.append("|");

			resultList.append(additionalString);
			resultList.append("|");
		}
		return resultList;
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
				QPair<qint64, double> additional = dataOfTime();
				mListOfTime.append(QString::number(additional.first));
				mListOfTime.append(" ");
				mListOfTime.append(QString::number(additional.second));
				mListOfTime.append("|");
				resultList.append(additionalString);
				resultList.append("|");
			}
		}
		return resultList;
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
					QPair<qint64, double> additional = dataOfTime();
					mListOfTime.append(QString::number(additional.first));
					mListOfTime.append(" ");
					mListOfTime.append(QString::number(additional.second));
					mListOfTime.append("|");
					resultList.append(additionalString);
					resultList.append("|");
				}
			}
		}
		return resultList;
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
						QPair<qint64, double> additional = dataOfTime();
						mListOfTime.append(QString::number(additional.first));
						mListOfTime.append(" ");
						mListOfTime.append(QString::number(additional.second));
						mListOfTime.append("|");
						resultList.append(additionalString);
						resultList.append("|");
					}
				}
			}
		}
		return resultList;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorId;
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertIdIntoStringList(editorId);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EditorsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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
		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;

		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->diagrams(editorId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;

		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->diagrams(editorId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->elements(diagramId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->elements(diagramId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsListGeneratorWithIdParameter();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->elements(editorName, diagramName);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;

		return editorManagerInterface->elements(editorName, diagramName);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsListGeneratorWithQStringParameters();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->mouseGesture(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->mouseGesture(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new MouseGesturesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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
		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				foreach (Id const &parentDiagram, editorManagerInterface->diagrams(editorId))
				{
					foreach (Id const &parentElement,
							 editorManagerInterface->elements(diagramId))
					{
						editorManagerInterface->isParentOf(elementId, parentElement);
					}
				}
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}

		variance = variance / 20;

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

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentOfGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->friendlyName(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}

		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->friendlyName(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new FriendlyNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->description(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}
		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->description(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DescriptionListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				try {
					editorManagerInterface->propertyDescription(elementId, propertyName);
				} catch (Exception e) {
				}
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}
		variance = variance / 20;

		QStringList result;
		try {
			result = ConvertingMethods::convertStringIntoStringList(
					editorManagerInterface->propertyDescription(elementId, propertyName));
		} catch (Exception e) {
			result.append("method failed");
		}
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDescriptionListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->propertyDisplayedName(elementId,propertyName);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->propertyDisplayedName(elementId,propertyName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDisplayedNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->containedTypes(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}

		variance = variance / 20;
		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->containedTypes(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ContainedTypesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->explosions(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertExplosionListIntoStringList(editorManagerInterface->explosions(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ExplosionsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndInterpreter::EnumValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->enumValues(elementId, name);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertingQPairListIntoStringList(editorManagerInterface->enumValues(elementId, name));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EnumValuesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->typeName(elementId, name);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}

		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->typeName(elementId, name));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new TypeNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->referenceProperties(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return (editorManagerInterface->referenceProperties(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ReferencePropertiesGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->allChildrenTypesOf(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return (editorManagerInterface->allChildrenTypesOf(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new AllChildrenTypesOfListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->isEditor(editorId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isEditor(editorId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsEditorListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->isDiagram(diagramId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isDiagram(diagramId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsDiagramListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->isElement(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isElement(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsElementListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->propertyNames(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return (editorManagerInterface->propertyNames(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyNamesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->portTypes(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return editorManagerInterface->portTypes(elementId);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PortTypesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->defaultPropertyValue(elementId, propertyName);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->defaultPropertyValue(elementId, propertyName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DefaultPropertyValuesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->propertiesWithDefaultValues(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return (editorManagerInterface->propertiesWithDefaultValues(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertiesWithDefaultValuesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->hasElement(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->hasElement(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new HasElementListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->findElementByType(type);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertIdIntoStringList(editorManagerInterface->findElementByType(type));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new FindElementByTypeListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->isGraphicalElementNode(elementId);
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		return ConvertingMethods::convertBoolIntoStringList(
				editorManagerInterface->isGraphicalElementNode(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsGraphicalElementListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->isNodeOrEdge(editorName, elementName);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertIntIntoStringList(
				editorManagerInterface->isNodeOrEdge(editorName, elementName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsNodeOrEdgeListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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
		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->diagramName(editorName, diagramName);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;

		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->diagramName(editorName, diagramName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->diagramNodeName(editorName, diagramName);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->diagramNodeName(editorName, diagramName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNodeNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->isParentProperty(elementId, propertyName);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertBoolIntoStringList(
				editorManagerInterface->isParentProperty(elementId, propertyName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentPropertyListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->children(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->children(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ChildrenListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->shape(elementId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->shape(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ShapeListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->paletteGroups(editorId, diagramId);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return editorManagerInterface->paletteGroups(editorId, diagramId);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupsGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups : public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;
		foreach (Id const &editor, editorManagerInterface->editors()) {
			foreach (Id const &diagram, editorManagerInterface->diagrams(editor)) {
					foreach (QString const &group, editorManagerInterface->paletteGroups(editor, diagram)) {
						QString const additionalString = ConvertingMethods::transformateOutput(
								callMethod(editorManagerInterface, editor, diagram, diagram, group), Id::rootId(), group);
						QPair<qint64, double> additional = dataOfTime();
						mListOfTime.append(QString::number(additional.first));
						mListOfTime.append(" ");
						mListOfTime.append(QString::number(additional.second));
						mListOfTime.append("|");

						resultList.append(additionalString);
						resultList.append("|");
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

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->paletteGroupList(editorId, diagramId, group);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;

		return editorManagerInterface->paletteGroupList(editorId, diagramId, group);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
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
		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorManagerInterface->paletteGroupDescription(editorId, diagramId, group);
			}
			array[i] = timer.nsecsElapsed();
			time += array[i];
		}
		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t)
		{
			variance +=qPow ((array[t] - expectedValue), 2);

		}
		variance = variance / 20;
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->paletteGroupDescription(editorId, diagramId, group));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupDescriptionGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}
private:

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

void MethodsTesterForQrxcAndInterpreter::testMethods()
{
	fillMethodsToTestList(pathToQrxcInterpreterMethodsToTest);

//	mGeneratedList.append(testMethodIfExistsInList(EditorsListGenerator(), "editors"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramsListGenerator(), "diagrams"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsListGeneratorWithIdParameter(), "elements(Id)"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsListGeneratorWithQStringParameters()
			, "elements(QString, QString)"));

	mGeneratedList.append(testMethodIfExistsInList(MouseGesturesListGenerator(), "mouseGestures"));
	mGeneratedList.append(testMethodIfExistsInList(FriendlyNameListGenerator(), "friendlyName"));
	mGeneratedList.append(testMethodIfExistsInList(DescriptionListGenerator(), "description"));
	//mGeneratedList.append(testMethodIfExistsInList(PropertyDescriptionListGenerator(), "propertyDescription"));
	mGeneratedList.append(testMethodIfExistsInList(PropertyDisplayedNameListGenerator(), "propertyDisplayedName"));

	mGeneratedList.append(testMethodIfExistsInList(ContainedTypesListGenerator(), "containedTypes"));
	mGeneratedList.append(testMethodIfExistsInList(ExplosionsListGenerator(), "explosions"));
	mGeneratedList.append(testMethodIfExistsInList(EnumValuesListGenerator(), "enumValues"));
	mGeneratedList.append(testMethodIfExistsInList(TypeNameListGenerator(), "typeName"));
	//mGeneratedList.append(testMethodIfExistsInList(AllChildrenTypesOfListGenerator(), "allChildrenTypesOf"));

	//mGeneratedList.append(testMethodIfExistsInList(IsEditorListGenerator(), "isEditor"));
	//mGeneratedList.append(testMethodIfExistsInList(IsDiagramListGenerator(), "isDiagram"));
	//mGeneratedList.append(testMethodIfExistsInList(IsElementListGenerator(), "isElement"));

	mGeneratedList.append(testMethodIfExistsInList(PropertyNamesListGenerator(), "propertyNames"));
	mGeneratedList.append(testMethodIfExistsInList(PortTypesListGenerator(), "portTypes"));
	mGeneratedList.append(testMethodIfExistsInList(DefaultPropertyValuesListGenerator(), "defaultPropertyValues"));
	mGeneratedList.append(testMethodIfExistsInList(PropertiesWithDefaultValuesListGenerator()
			, "propertiesWithDefaultValues"));

	//mGeneratedList.append(testMethodIfExistsInList(HasElementListGenerator(), "hasElement"));
	//mGeneratedList.append(testMethodIfExistsInList(FindElementByTypeListGenerator(), "findElementByType"));
	//mGeneratedList.append(testMethodIfExistsInList(IsGraphicalElementListGenerator(), "isGraphicalElement"));

	mGeneratedList.append(testMethodIfExistsInList(IsNodeOrEdgeListGenerator(), "isNodeOrEdge"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNameListGenerator(), "diagramName"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNodeNameListGenerator(), "diagramNodeName"));
	//mGeneratedList.append(testMethodIfExistsInList(IsParentPropertyListGenerator(), "isParentProperty"));
	//mGeneratedList.append(testMethodIfExistsInList(ChildrenListGenerator(), "children"));
	//mGeneratedList.append(testMethodIfExistsInList(ShapeListGenerator(), "shape"));
	//paletteGroups PaletteGroupsGenerator
//	mGeneratedList.append(testMethodIfExistsInList(PaletteGroupsGenerator(), "paletteGroups"));
//	mGeneratedList.append(testMethodIfExistsInList(PaletteGroupsListGenerator(), "paletteGroupList"));
	//mGeneratedList.append(testMethodIfExistsInList(PaletteGroupDescriptionGenerator(), "paletteGroupDescription"));
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
