/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>

#include "convertingMethods.h"
#include "methodsTesterForQrxcAndQrmc.h"
#include "abstractStringGenerator.h"
#include "defs.h"
#include "qrgui/plugins/metaMetaModel/metamodel.h"

#include <iostream>
#include <QElapsedTimer>
#include <QtCore/qmath.h>

using namespace std;

using namespace qReal;
using namespace editorPluginTestingFramework;

MethodsTesterForQrxcAndQrmc::MethodsTesterForQrxcAndQrmc(
		Metamodel * const qrmcGeneratedPlugin
		, Metamodel * const qrxcGeneratedPlugin
		)
{
	mQrmcGeneratedPlugin = qrmcGeneratedPlugin;
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
}

class MethodsTesterForQrxcAndQrmc::StringGenerator : public AbstractStringGenerator
{
public:
	void init(Metamodel *editorInterface) {
		mEditorInterface = editorInterface;
	}

	virtual QString generateString() const
	{
		QString resultStr = "";

		for (const QString &elementOfList: generateList(mEditorInterface)) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}

	virtual QString generateStringTime()
	{
		QString resultStr = "";
		resultStr = mListOfTime.join(' ');
		return resultStr;
	}

	virtual QPair<qint64, double> callFunction(std::function<void()> functionToCall) const
	{
		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		qint64 expectedValue = 0;
		qint64 variance = 0;

		for (int i = 0; i < 20; ++i) {
			timer.start();
			for (int i = 0; i < 20; ++i) {
				functionToCall();
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t) {
			variance +=qPow ((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		double sqrtVariance = sqrt(variance);

		QPair<qint64, double> result = qMakePair(expectedValue, sqrtVariance);
		return result;
	}

	virtual QString methodName() const = 0;

protected:
	virtual QStringList generateList(Metamodel *editorInterface) const = 0;

	virtual QPair<qint64, double> dataOfTime() const = 0;

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element = ""
			, const QString &property = ""
			) const = 0;

	QStringList mutable mListOfTime;

private:
	Metamodel *mEditorInterface;
};

class MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams : public MethodsTesterForQrxcAndQrmc::StringGenerator
{
	virtual QStringList generateList(Metamodel *editorInterface) const {
		QStringList resultList;
		for (const QString &diagram : editorInterface->diagrams()) {
			const QString additionalString = ConvertingMethods::transformateOutput(
					callMethod(editorInterface, diagram, "", ""), Id::rootId(), diagram);
			const QPair<qint64, double> additional = dataOfTime();
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
	virtual QStringList generateList(Metamodel *editorInterface) const {
		QStringList resultList;
		for (const QString &diagram : editorInterface->diagrams()) {
			for (const QString &element : editorInterface->elements(diagram)) {
				const QString additionalString = ConvertingMethods::transformateOutput(
						callMethod(editorInterface, diagram, element, ""), Id::rootId(), element);
				const QPair<qint64, double> additional = dataOfTime();
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
	virtual QStringList generateList(Metamodel *editorInterface) const {
		QStringList resultList;
		for (const QString &diagram : editorInterface->diagrams()) {
			for (const QString &element : editorInterface->elements(diagram)) {
				for (const QString &property : editorInterface->getPropertyNames(diagram, element)) {
					const QString additionalString = ConvertingMethods::transformateOutput(callMethod(
							editorInterface, diagram, element, property), Id::rootId(), property + "(" + element + ")");
					const QPair<qint64, double> additional = dataOfTime();
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
	virtual QStringList generateList(Metamodel *editorInterface) const {
		QStringList resultList;

		for (const QString &diagram: editorInterface->diagrams()) {
			for (const QString &group: editorInterface->diagramPaletteGroups(diagram)) {
				const QString &additionalString = ConvertingMethods::transformateOutput(
						callMethod(editorInterface, diagram, group), Id::rootId(), group);
				const QPair<qint64, double> additional = dataOfTime();
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
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);
		mResult = callFunction([editorInterface, diagram]() { return editorInterface->elements(diagram); });

		return editorInterface->elements(diagram);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::ExplosionsStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Explosions";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->explosions(diagram, element); });

		QStringList result;
		result = ConvertingMethods::convertQListExplosionDataIntoStringList(editorInterface->explosions(diagram, element));
		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ExplosionsStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::PortTypesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Port Types";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->getPortTypes(diagram, element); });

		return editorInterface->getPortTypes(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PortTypesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:

	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::PropertiesWithDefaultValuesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Properties with default values";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, element]()
				{ return editorInterface->getPropertiesWithDefaultValues(element); });

		return editorInterface->getPropertiesWithDefaultValues(element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertiesWithDefaultValuesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::TypesContainedByStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Types contained by";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, element]()
				{ return editorInterface->getTypesContainedBy(element); });

		return editorInterface->getTypesContainedBy(element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new TypesContainedByStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
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
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, element]()
				{ return editorInterface->getPossibleEdges(element); });

		QStringList result;
		for (const PossibleEdgesType &pair: editorInterface->getPossibleEdges(element)) {
			const QString &firstElement = pair.first.first;
			const QString &secondElement = pair.first.second;

			const QString &thirdElement = QString("%1").arg(pair.second.first);
			const QString &fourthElement = pair.second.second;

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
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::IsNodeOrEdgeStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is node or edge";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, element]()
				{ return editorInterface->isNodeOrEdge(element); });

		return ConvertingMethods::convertIntIntoStringList(editorInterface->isNodeOrEdge(element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsNodeOrEdgeStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::GetPropertyNamesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Get property names";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->getPropertyNames(diagram, element); });

		return editorInterface->getPropertyNames(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyNamesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::GetReferencePropertiesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Get reference properties";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->getReferenceProperties(diagram, element); });

		return editorInterface->getReferenceProperties(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetReferencePropertiesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};


class MethodsTesterForQrxcAndQrmc::GetPropertyTypesStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Get property types";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		mResult = callFunction([editorInterface, element, property]()
				{ return editorInterface->getPropertyType(element, property); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyType(element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyTypesStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::GetPropertyDefaultValueStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Get property default values";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		mResult = callFunction([editorInterface, element, property]()
				{ return editorInterface->getPropertyDefaultValue(element, property); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyDefaultValue(element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyDefaultValueStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::EnumValueStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Enum values";
	}

	virtual void callEnum(Metamodel *editorInterface, const QString &diagram
			, const QString &element) const
	{
		QStringList propertyNames = editorInterface->getPropertyNames(diagram, element);
		for (const QString &name : propertyNames) {
			const QString &type = editorInterface->getPropertyType(element, name);
			editorInterface->getEnumValues(type);
		}

	}


	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		mResult = callFunction([this, editorInterface, diagram, element]()
				{ return callEnum(editorInterface, diagram, element); });

		QStringList propertyNames = editorInterface->getPropertyNames(diagram, element);
		QList<QPair<QString, QString>> result;
		for (const QString &name : propertyNames) {
			const QString &type = editorInterface->getPropertyType(element, name);
			result.append(editorInterface->getEnumValues(type));
		}

		return ConvertingMethods::convertingQPairListIntoStringList(result);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EnumValueStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
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
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->getParentsOf(diagram, element); });

		QStringList result;

		result << element << ": ";
		for (const ParentsOfType &pair: editorInterface->getParentsOf(diagram, element)) {
			const QString &firstElement = pair.first;
			const QString &secondElement = pair.second;
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
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::DiagramsStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagrams";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);
		Q_UNUSED(diagram);
		mResult = callFunction([editorInterface]() { return editorInterface->diagrams(); });
		return editorInterface->diagrams();
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramsStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::DiagramNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram name";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);
		mResult = callFunction([editorInterface, diagram]()
				{ return editorInterface->diagramName(diagram); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramName(diagram));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::DiagramNodeNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram node name";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		Q_UNUSED(element);
		mResult = callFunction([editorInterface, diagram]()
				{ return editorInterface->diagramNodeName(diagram); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramNodeName(diagram));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNodeNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::ElementNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Element name";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->elementName(diagram, element); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementName(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}
private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::ElementMouseGestureStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Element mouse gesture";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->elementMouseGesture(diagram, element); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementMouseGesture(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementMouseGestureStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::ElementDescriptionStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Element description";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->elementDescription(diagram, element); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementDescription(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementDescriptionStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::PropertyDescriptionStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property description";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		mResult = callFunction([editorInterface, diagram, element, property]()
				{ return editorInterface->propertyDescription(diagram, element, property); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDescription(diagram, element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDescriptionStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::PropertyDisplayedNameStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property displayed name";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		mResult = callFunction([editorInterface, diagram, element, property]()
				{ return editorInterface->propertyDisplayedName(diagram, element, property); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDisplayedName(diagram, element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDisplayedNameStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::IsParentOfStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is parent of";
	}

	virtual void callIsParent(Metamodel *editorInterface, const QString &diagram
			, const QString &element) const
	{
		for (const QString &parentDiagram: editorInterface->diagrams()) {
			for (const QString &parentElement: editorInterface->elements(diagram)) {
				editorInterface->isParentOf(parentDiagram, parentElement, diagram, element);
			}
		}
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		mResult = callFunction([this, editorInterface, diagram, element]()
				{ callIsParent(editorInterface, diagram, element); });

		Q_UNUSED(property);
		QStringList result;
		for (const QString &parentDiagram : editorInterface->diagrams()) {
			for (const QString &parentElement : editorInterface->elements(diagram)) {
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
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::DiagramPaletteGroupListStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Diagram palette group list";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->diagramPaletteGroupList(diagram, element); });

		return editorInterface->diagramPaletteGroupList(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramPaletteGroupListStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndQrmc::DiagramPaletteGroupDescriptionStringGenerator
		: public MethodsTesterForQrxcAndQrmc::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Diagram palette group description";
	}

	virtual QStringList callMethod(
			Metamodel *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			) const
	{
		Q_UNUSED(property);
		mResult = callFunction([editorInterface, diagram, element]()
				{ return editorInterface->diagramPaletteGroupDescription(diagram, element); });

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramPaletteGroupDescription(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramPaletteGroupDescriptionStringGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

QList<MethodsTester::ResultOfGenerating> MethodsTesterForQrxcAndQrmc::generatedOutput()
{
	testMethods();
	return mGeneratedList;
}

void MethodsTesterForQrxcAndQrmc::testMethods()
{
	fillMethodsToTestList(pathToQrxcQrmcMethodsToTest);
	mGeneratedList.append(testMethodIfExistsInList(DiagramsStringGenerator(), "diagrams"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsStringGenerator(), "elements"));
	mGeneratedList.append(testMethodIfExistsInList(PortTypesStringGenerator(), "getPortTypes"));
	mGeneratedList.append(testMethodIfExistsInList(EnumValueStringGenerator(), "getEnumValues"));
	mGeneratedList.append(testMethodIfExistsInList(PropertiesWithDefaultValuesStringGenerator()
			, "propertiesWithDefaulValues"));
	mGeneratedList.append(testMethodIfExistsInList(IsNodeOrEdgeStringGenerator(), "isNodeOrEdge"));
	mGeneratedList.append(testMethodIfExistsInList(GetPropertyNamesStringGenerator(), "getPropertyNames"));
	mGeneratedList.append(testMethodIfExistsInList(GetPropertyDefaultValueStringGenerator(), "getPropertyDefaultValue"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNameStringGenerator(), "diagramName"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNodeNameStringGenerator(), "diagramNodeName"));
	mGeneratedList.append(testMethodIfExistsInList(ElementNameStringGenerator(), "elementName"));
	mGeneratedList.append(testMethodIfExistsInList(ElementMouseGestureStringGenerator(), "elementMouseGesture"));
	mGeneratedList.append(testMethodIfExistsInList(ElementDescriptionStringGenerator(), "elementDescription"));
	mGeneratedList.append(testMethodIfExistsInList(PropertyDisplayedNameStringGenerator(), "propertyDisplayedName"));
	mGeneratedList.append(testMethodIfExistsInList(IsParentOfStringGenerator(), "isParentOf"));
}

bool MethodsTesterForQrxcAndQrmc::containsOnly(const QString &string, QChar const &symbol)
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
