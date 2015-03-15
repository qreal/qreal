#include <QtCore/QDebug>
#include <QtWidgets/QApplication>

#include "convertingMethods.h"
#include "methodsTesterForQrxcAndQrmc.h"
#include "abstractStringGenerator.h"
#include "defs.h"
#include "qrgui/plugins/editorPluginInterface/editorInterface.h"

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

	virtual QString methodName() const = 0;

protected:
	virtual QStringList generateList(EditorInterface *editorInterface) const = 0;
	virtual QStringList generateListTime(EditorInterface *editorInterface) = 0;


	virtual QStringList callMethod(
			EditorInterface *editorInterface
			, QString const &diagram
			, QString const &element = ""
			, QString const &property = ""
			) const = 0;

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, QString const &diagram
			, QString const &element = ""
			, QString const &property = ""
			) = 0;



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
			resultList.append(additionalString);
			resultList.append("|");
		}

		return resultList;
	}



	virtual QStringList generateListTime(EditorInterface *editorInterface)  {
		QStringList resultList;
		foreach (QString const &diagram, editorInterface->diagrams()) {
			QString const additionalString = ConvertingMethods::transformateOutput(
					callMethodTime(editorInterface, diagram, "", ""), Id::rootId(), diagram);
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
				resultList.append(additionalString);
				resultList.append("|");
			}
		}

		return resultList;
	}


	virtual QStringList generateListTime(EditorInterface *editorInterface)  {
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &element, editorInterface->elements(diagram)) {
				QString const additionalString = ConvertingMethods::transformateOutput(
						callMethodTime(editorInterface, diagram, element, ""), Id::rootId(), element);
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
					resultList.append(additionalString);
					resultList.append("|");
				}
			}
		}

		return resultList;
	}



	virtual QStringList generateListTime(EditorInterface *editorInterface) {
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &element, editorInterface->elements(diagram)) {
				foreach (QString const &property, editorInterface->getPropertyNames(diagram, element)) {
					QString const additionalString = ConvertingMethods::transformateOutput(callMethodTime(
							editorInterface, diagram, element, property), Id::rootId(), property + "(" + element + ")");
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
				resultList.append(additionalString);
				resultList.append("|");
			}
		}

		return resultList;
	}


	virtual QStringList generateListTime(EditorInterface *editorInterface)  {
		QStringList resultList;

		foreach (QString const &diagram, editorInterface->diagrams()) {
			foreach (QString const &group, editorInterface->diagramPaletteGroups(diagram)) {
				QString const additionalString = ConvertingMethods::transformateOutput(
						callMethodTime(editorInterface, diagram, group), Id::rootId(), group);
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
		return editorInterface->elements(diagram);
	}



	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->elements(diagram);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->elements(diagram);
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsStringGenerator();
		return clonedGenerator;
	}
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
		//result.append("ololo");
		Q_UNUSED(property);
		//ExplosionData
		QList<EditorInterface::ExplosionData> ololo = editorInterface->explosions(diagram, element);
		result = ConvertingMethods::convertQListExplosionDataIntoStringList(ololo);

		return result; //ConvertingMethods::convertExplosionListIntoStringList(editorInterface->explosions(diagram, element));
	}


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		QStringList result;
		//result.append("ololo");
		Q_UNUSED(property);
		//ExplosionData

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();

			QList<EditorInterface::ExplosionData> ololo = editorInterface->explosions(diagram, element);
			result = ConvertingMethods::convertQListExplosionDataIntoStringList(ololo);

			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		QList<EditorInterface::ExplosionData> ololo = editorInterface->explosions(diagram, element);
		result = ConvertingMethods::convertQListExplosionDataIntoStringList(ololo);

		return result; //ConvertingMethods::convertExplosionListIntoStringList(editorInterface->explosions(diagram, element));
	}



	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ExplosionsStringGenerator();
		return clonedGenerator;
	}
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
		return editorInterface->getPortTypes(diagram, element);
	}


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getPortTypes(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->getPortTypes(diagram, element);
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PortTypesStringGenerator();
		return clonedGenerator;
	}

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
		return editorInterface->getPropertiesWithDefaultValues(element);
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getPropertiesWithDefaultValues(element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->getPropertiesWithDefaultValues(element);
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertiesWithDefaultValuesStringGenerator();
		return clonedGenerator;
	}
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
		return editorInterface->getTypesContainedBy(element);
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getTypesContainedBy(element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->getTypesContainedBy(element);
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new TypesContainedByStringGenerator();
		return clonedGenerator;
	}
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


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);
		QStringList result;

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();

			foreach (PossibleEdgesType const &pair, editorInterface->getPossibleEdges(element)) {
				QString const &firstElement = pair.first.first;
				QString const &secondElement = pair.first.second;

				QString const &thirdElement = QString("%1").arg(pair.second.first);
				QString const &fourthElement = pair.second.second;

				result << firstElement << secondElement << thirdElement << fourthElement;
			}

			time += timer.nsecsElapsed();
		}
		time = time / 1000;


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
		return ConvertingMethods::convertIntIntoStringList(editorInterface->isNodeOrEdge(element));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->isNodeOrEdge(element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertIntIntoStringList(editorInterface->isNodeOrEdge(element));
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsNodeOrEdgeStringGenerator();
		return clonedGenerator;
	}
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
		return editorInterface->getPropertyNames(diagram, element);
	}


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getPropertyNames(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->getPropertyNames(diagram, element);
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyNamesStringGenerator();
		return clonedGenerator;
	}
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
		return editorInterface->getReferenceProperties(diagram, element);
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getReferenceProperties(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->getReferenceProperties(diagram, element);
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetReferencePropertiesStringGenerator();
		return clonedGenerator;
	}
};

// !!!!!!!!!!!!!!!! getParentsOf




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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyType(element, property));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getPropertyType(element, property);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyType(element, property));
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyTypesStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyDefaultValue(element, property));
	}


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getPropertyDefaultValue(element, property);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->getPropertyDefaultValue(element, property));
	}



	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new GetPropertyDefaultValueStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertingQPairListIntoStringList(editorInterface->getEnumValues(diagram));
	}


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(diagram);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->getEnumValues(diagram);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertingQPairListIntoStringList(editorInterface->getEnumValues(diagram));
	}



	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EnumValueStringGenerator();
		return clonedGenerator;
	}
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

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);
		QStringList result;

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();

			result << element << ": ";
			foreach (ParentsOfType const &pair, editorInterface->getParentsOf(diagram, element)) {
				QString const &firstElement = pair.first;
				QString const &secondElement = pair.second;

				result << firstElement << secondElement;
			}
			result << "\n";

			time += timer.nsecsElapsed();
		}
		time = time / 1000;

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

		return editorInterface->diagrams();
	}


	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);
		Q_UNUSED(element);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->diagrams();
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->diagrams();
	}



	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramsStringGenerator();
		return clonedGenerator;
	}
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

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramName(diagram));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);
		Q_UNUSED(element);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->diagramName(diagram);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramName(diagram));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNameStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramNodeName(diagram));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);
		Q_UNUSED(element);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->diagramNodeName(diagram);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramNodeName(diagram));
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNodeNameStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementName(diagram, element));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->elementName(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementName(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementNameStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementMouseGesture(diagram, element));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->elementMouseGesture(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementMouseGesture(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementMouseGestureStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementDescription(diagram, element));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->elementDescription(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->elementDescription(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementDescriptionStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDescription(diagram, element, property));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->propertyDescription(diagram, element, property);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;
		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDescription(diagram, element, property));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDescriptionStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDisplayedName(diagram, element, property));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->propertyDisplayedName(diagram, element, property);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;
		return ConvertingMethods::convertStringIntoStringList(editorInterface->propertyDisplayedName(diagram, element, property));
	}


	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDisplayedNameStringGenerator();
		return clonedGenerator;
	}
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

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);
		QStringList result;

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();

			foreach (QString const &parentDiagram, editorInterface->diagrams()) {
				foreach (QString const &parentElement, editorInterface->elements(diagram)) {
					bool isParent = editorInterface->isParentOf(parentDiagram, parentElement, diagram, element);
					if (isParent) {
						result << parentElement << " is parent of " << element << "\n";
					}
				}
			}

			time += timer.nsecsElapsed();
		}
		time = time / 1000;

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
		return editorInterface->diagramPaletteGroupList(diagram, element);
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->diagramPaletteGroupList(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return editorInterface->diagramPaletteGroupList(diagram, element);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramPaletteGroupListStringGenerator();
		return clonedGenerator;
	}
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
		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramPaletteGroupDescription(diagram, element));
	}

	virtual QStringList callMethodTime(
			EditorInterface *editorInterface
			, const QString &diagram
			, const QString &element
			, const QString &property
			)
	{
		Q_UNUSED(property);

		qint64 time = 0;
		QElapsedTimer timer;
		for (int i = 0; i < 1000; ++i)
		{
			timer.start();
			editorInterface->diagramPaletteGroupDescription(diagram, element);
			time += timer.nsecsElapsed();
		}
		time = time / 1000;

		return ConvertingMethods::convertStringIntoStringList(editorInterface->diagramPaletteGroupDescription(diagram, element));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramPaletteGroupDescriptionStringGenerator();
		return clonedGenerator;
	}
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
	mGeneratedList.append(testMethodIfExistsInList(GetPossibleEdgesStringGenerator(), "getPossibleEdges"));
	mGeneratedList.append(testMethodIfExistsInList(IsNodeOrEdgeStringGenerator(), "isNodeOrEdge"));

	mGeneratedList.append(testMethodIfExistsInList(GetPropertyNamesStringGenerator(), "getPropertyNames"));
	mGeneratedList.append(testMethodIfExistsInList(GetReferencePropertiesStringGenerator(), "getReferenceProperties"));
	mGeneratedList.append(testMethodIfExistsInList(GetParentsOfStringGenerator(), "getParentsOf"));

	mGeneratedList.append(testMethodIfExistsInList(GetPropertyTypesStringGenerator(), "getPropertyTypes"));
	mGeneratedList.append(testMethodIfExistsInList(GetPropertyDefaultValueStringGenerator(), "getPropertyDefaultValue"));

	mGeneratedList.append(testMethodIfExistsInList(DiagramNameStringGenerator(), "diagramName"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNodeNameStringGenerator(), "diagramNodeName"));

	mGeneratedList.append(testMethodIfExistsInList(ElementNameStringGenerator(), "elementName"));
	mGeneratedList.append(testMethodIfExistsInList(ElementMouseGestureStringGenerator(), "elementMouseGesture"));
	mGeneratedList.append(testMethodIfExistsInList(ElementDescriptionStringGenerator(), "elementDescription"));

	mGeneratedList.append(testMethodIfExistsInList(PropertyDisplayedNameStringGenerator(), "propertyDisplayedName"));
	mGeneratedList.append(testMethodIfExistsInList(PropertyDescriptionStringGenerator(), "propertyDescription"));

	//mGeneratedList.append(testMethodIfExistsInList(IsParentOfStringGenerator(), "isParentOf"));

	mGeneratedList.append(testMethodIfExistsInList(DiagramPaletteGroupListStringGenerator(), "diagramPaletteGroup"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramPaletteGroupDescriptionStringGenerator()
			, "diagramPaletteGroupDescription"));
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
