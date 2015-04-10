#include <QtCore/QDebug>
#include <QtWidgets/QApplication>

#include "convertingMethods.h"
#include "methodsTesterForQrxcAndQrmc.h"
#include "abstractStringGenerator.h"
#include "defs.h"
#include "qrgui/plugins/editorPluginInterface/editorInterface.h"

#include <iostream>
#include <QElapsedTimer>
#include <QtCore/qmath.h>

using namespace std;

using namespace qReal;
using namespace editorPluginTestingFramework;

//using namespace ExplosionData;

MethodsTesterForQrxcAndQrmc::MethodsTesterForQrxcAndQrmc(
		EditorInterface * const qrmcGeneratedPlugin
		, EditorInterface * const qrxcGeneratedPlugin
		)
{
	mQrmcGeneratedPlugin = qrmcGeneratedPlugin;
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
}

class MethodsTesterForQrxcAndQrmc::StringGenerator : public AbstractStringGenerator
{
public:

	void init(EditorInterface *editorInterface) {
		mEditorInterface = editorInterface;
	}

	virtual QString generateString() const
	{
		QString resultStr = "";

		foreach (QString const &elementOfList, generateList(mEditorInterface)) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}


	virtual QString generateStringTime()
	{
		QString resultStr = "";

		foreach (QString elementOfList, mListOfTime) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}

	virtual QString methodName() const = 0;

protected:
	virtual QStringList generateList(EditorInterface *editorInterface) const = 0;

	virtual QPair<qint64, double> dataOfTime() const = 0;

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, QString const &diagram
			, QString const &element = ""
			, QString const &property = ""
			) const = 0;

	QStringList mutable mListOfTime;
private:
	EditorInterface *mEditorInterface;
};

class MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams : public MethodsTesterForQrxcAndQrmc::StringGenerator
{
	virtual QStringList generateList(EditorInterface *editorInterface) const {
		QStringList resultList;
		foreach (QString const &diagram, editorInterface->diagrams()) {
			QString const additionalString = ConvertingMethods::transformateOutput(
					callMethod(editorInterface, diagram, "", ""), Id::rootId(), diagram);
			QPair<qint64, double> additional = dataOfTime();
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

class MethodsTesterForQrxcAndQrmc::StringGeneratorForElements : public MethodsTesterForQrxcAndQrmc::StringGenerator
{
	virtual QStringList generateList(EditorInterface *editorInterface) const {
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &element, editorInterface->elements(diagram)) {
				QString const additionalString = ConvertingMethods::transformateOutput(
						callMethod(editorInterface, diagram, element, ""), Id::rootId(), element);
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

class MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties : public MethodsTesterForQrxcAndQrmc::StringGenerator
{
	virtual QStringList generateList(EditorInterface *editorInterface) const {
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &element, editorInterface->elements(diagram)) {
				foreach (QString const &property, editorInterface->getPropertyNames(diagram, element)) {
					QString const additionalString = ConvertingMethods::transformateOutput(callMethod(
							editorInterface, diagram, element, property), Id::rootId(), property + "(" + element + ")");
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

class MethodsTesterForQrxcAndQrmc::StringGeneratorForGroups : public MethodsTesterForQrxcAndQrmc::StringGenerator
{
	virtual QStringList generateList(EditorInterface *editorInterface) const {
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &group, editorInterface->diagramPaletteGroups(diagram)) {
				QString const additionalString = ConvertingMethods::transformateOutput(
						callMethod(editorInterface, diagram, group), Id::rootId(), group);
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

class MethodsTesterForQrxcAndQrmc::ElementsStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Elements";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->elements(diagram);
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
		return editorInterface->elements(diagram);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}


	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::ExplosionsStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Explosions";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		QStringList result;
		Q_UNUSED(property);
		QElapsedTimer timer;

		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->explosions(diagram, element);
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

		QList<EditorInterface::ExplosionData> ololo = editorInterface->explosions(diagram, element);
		result = ConvertingMethods::convertQListExplosionDataIntoStringList(ololo);
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ExplosionsStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::PortTypesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Port Types";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getPortTypes(diagram, element);
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
		return editorInterface->getPortTypes(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PortTypesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::PropertiesWithDefaultValuesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Properties with default values";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getPropertiesWithDefaultValues(element);
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
		return editorInterface->getPropertiesWithDefaultValues(element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertiesWithDefaultValuesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::TypesContainedByStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Types contained by";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getTypesContainedBy(element);
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
		return editorInterface->getTypesContainedBy(element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new TypesContainedByStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::GetPossibleEdgesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	typedef QPair<QPair<QString, QString>, QPair<bool, QString> > PossibleEdgesType;

	virtual QString methodName() const
	{
		return "Get possible edges";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				foreach (PossibleEdgesType const &pair, editorInterface->getPossibleEdges(element)) {
					pair.first.first;
					pair.first.second;

					QString("%1").arg(pair.second.first);
					pair.second.second;
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
		foreach (PossibleEdgesType const &pair, editorInterface->getPossibleEdges(element)) {
			QString const &firstElement = pair.first.first;
			QString const &secondElement = pair.first.second;

			QString const &thirdElement = QString("%1").arg(pair.second.first);
			QString const &fourthElement = pair.second.second;

			result << firstElement << secondElement << thirdElement << fourthElement;
		}
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPossibleEdgesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::IsNodeOrEdgeStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is node or edge";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->isNodeOrEdge(element);
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
		return ConvertingMethods::convertIntIntoStringList(editorInterface->isNodeOrEdge(element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsNodeOrEdgeStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::GetPropertyNamesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Get property names";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getPropertyNames(diagram, element);
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
		return editorInterface->getPropertyNames(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyNamesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::GetReferencePropertiesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Get reference properties";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getReferenceProperties(diagram, element);
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
		return editorInterface->getReferenceProperties(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetReferencePropertiesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};


class MethodsTesterForQrxcAndQrmc::GetPropertyTypesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Get property types";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getPropertyType(element, property);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyType(element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyTypesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::GetPropertyDefaultValueStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Get property default values";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getPropertyDefaultValue(element, property);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyDefaultValue(element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyDefaultValueStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::EnumValueStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Enum values";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->getEnumValues(diagram);
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
		return ConvertingMethods::convertingQPairListIntoStringList(editorInterface->getEnumValues(diagram));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EnumValueStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::GetParentsOfStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	typedef QPair<QString, QString> ParentsOfType;

	virtual QString methodName() const
	{
		return "Get parents of";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				foreach (ParentsOfType const &pair, editorInterface->getParentsOf(diagram, element)) {
					pair.first;
					pair.second;
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

		result << element << ": ";
		foreach (ParentsOfType const &pair, editorInterface->getParentsOf(diagram, element)) {
			QString const &firstElement = pair.first;
			QString const &secondElement = pair.second;

			result << firstElement << secondElement;
		}
		result << "\n";
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetParentsOfStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::DiagramsStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagrams";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->diagrams();
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

		return editorInterface->diagrams();
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramsStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::DiagramNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram name";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->diagramName(diagram);
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

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramName(diagram));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::DiagramNodeNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram node name";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->diagramNodeName(diagram);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramNodeName(diagram));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNodeNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::ElementNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Element name";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->elementName(diagram, element);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementName(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::ElementMouseGestureStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Element mouse gesture";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->elementMouseGesture(diagram, element);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementMouseGesture(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementMouseGestureStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::ElementDescriptionStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Element description";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->elementDescription(diagram, element);
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

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementDescription(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementDescriptionStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::PropertyDescriptionStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property description";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
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
				editorInterface->propertyDescription(diagram, element, property);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDescription(diagram, element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDescriptionStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::PropertyDisplayedNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property displayed name";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
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
				editorInterface->propertyDisplayedName(diagram, element, property);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDisplayedName(diagram, element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDisplayedNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::IsParentOfStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is parent of";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
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
				foreach (QString const &parentDiagram, editorInterface->diagrams()) {
					foreach (QString const &parentElement, editorInterface->elements(diagram)) {
						bool isParent = editorInterface->isParentOf(parentDiagram, parentElement, diagram, element);
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

		Q_UNUSED(property);
		QStringList result;
		foreach (QString const &parentDiagram, editorInterface->diagrams()) {
			foreach (QString const &parentElement, editorInterface->elements(diagram)) {
				bool isParent = editorInterface->isParentOf(parentDiagram, parentElement, diagram, element);
				if (isParent) {
					result << parentElement << " is parent of " << element << "\n";
				}
			}
		}

		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentOfStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::DiagramPaletteGroupListStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Diagram palette group list";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->diagramPaletteGroupList(diagram, element);
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
		return editorInterface->diagramPaletteGroupList(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramPaletteGroupListStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

class MethodsTesterForQrxcAndQrmc::DiagramPaletteGroupDescriptionStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Diagram palette group description";
	}

	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);

		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		for (int i = 0; i < 20; ++i)
		{
			timer.start();
			for (int i = 0; i < 20; ++i)
			{
				editorInterface->diagramPaletteGroupDescription(diagram, element);
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramPaletteGroupDescription(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramPaletteGroupDescriptionStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		QPair<qint64, double> result = qMakePair(expectedValue, sqrt(variance));
		return result;
	}

	mutable qint64 expectedValue = 0;
	mutable qint64 variance = 0;
};

QList<QPair<QString, QPair<QString, QString> > > MethodsTesterForQrxcAndQrmc::generatedOutput()
{
	testMethods();
	return mGeneratedList;
}


void MethodsTesterForQrxcAndQrmc::testMethods()
{
	fillMethodsToTestList(pathToQrxcQrmcMethodsToTest);
	mGeneratedList.append(testMethodIfExistsInList(DiagramsStringGenerator(), "diagrams"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsStringGenerator(), "elements"));
	mGeneratedList.append(testMethodIfExistsInList(ExplosionsStringGenerator(), "explosions"));
	mGeneratedList.append(testMethodIfExistsInList(PortTypesStringGenerator(), "getPortTypes"));
	mGeneratedList.append(testMethodIfExistsInList(EnumValueStringGenerator(), "getEnumValues"));


	mGeneratedList.append(testMethodIfExistsInList(PropertiesWithDefaultValuesStringGenerator()
			, "propertiesWithDefaulValues"));
	mGeneratedList.append(testMethodIfExistsInList(TypesContainedByStringGenerator(), "typesContanedBy"));
//	mGeneratedList.append(testMethodIfExistsInList(GetPossibleEdgesStringGenerator(), "getPossibleEdges"));
	mGeneratedList.append(testMethodIfExistsInList(IsNodeOrEdgeStringGenerator(), "isNodeOrEdge"));

	mGeneratedList.append(testMethodIfExistsInList(GetPropertyNamesStringGenerator(), "getPropertyNames"));
	mGeneratedList.append(testMethodIfExistsInList(GetReferencePropertiesStringGenerator(), "getReferenceProperties"));
//	mGeneratedList.append(testMethodIfExistsInList(GetParentsOfStringGenerator(), "getParentsOf"));

//	mGeneratedList.append(testMethodIfExistsInList(GetPropertyTypesStringGenerator(), "getPropertyTypes"));
	mGeneratedList.append(testMethodIfExistsInList(GetPropertyDefaultValueStringGenerator(), "getPropertyDefaultValue"));

	mGeneratedList.append(testMethodIfExistsInList(DiagramNameStringGenerator(), "diagramName"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNodeNameStringGenerator(), "diagramNodeName"));

	mGeneratedList.append(testMethodIfExistsInList(ElementNameStringGenerator(), "elementName"));
	mGeneratedList.append(testMethodIfExistsInList(ElementMouseGestureStringGenerator(), "elementMouseGesture"));
	mGeneratedList.append(testMethodIfExistsInList(ElementDescriptionStringGenerator(), "elementDescription"));

	mGeneratedList.append(testMethodIfExistsInList(PropertyDisplayedNameStringGenerator(), "propertyDisplayedName"));
	//mGeneratedList.append(testMethodIfExistsInList(PropertyDescriptionStringGenerator(), "propertyDescription"));

	mGeneratedList.append(testMethodIfExistsInList(IsParentOfStringGenerator(), "isParentOf"));

//	mGeneratedList.append(testMethodIfExistsInList(DiagramPaletteGroupListStringGenerator(), "diagramPaletteGroup"));
//	mGeneratedList.append(testMethodIfExistsInList(DiagramPaletteGroupDescriptionStringGenerator()
//			, "diagramPaletteGroupDescription"));
}

bool MethodsTesterForQrxcAndQrmc::containsOnly(QString const &string, QChar const &symbol)
{
	bool containsOnlyThisSymbol = true;

	for (int i = 0; i < string.length(); i++) {
		if (string[i] != symbol) {
				containsOnlyThisSymbol = false;
		}
	}

	return containsOnlyThisSymbol;
}

AbstractStringGenerator * MethodsTesterForQrxcAndQrmc::initGeneratorWithFirstInterface(
		AbstractStringGenerator const &generator) const
{
	StringGenerator *clonedGenerator = dynamic_cast<StringGenerator *>(generator.clone());

	clonedGenerator->init(mQrxcGeneratedPlugin);
	return clonedGenerator;
}

AbstractStringGenerator * MethodsTesterForQrxcAndQrmc::initGeneratorWithSecondInterface(
		AbstractStringGenerator const &generator) const
{
	StringGenerator *clonedGenerator = dynamic_cast<StringGenerator *>(generator.clone());

	clonedGenerator->init(mQrmcGeneratedPlugin);
	return clonedGenerator;
}
