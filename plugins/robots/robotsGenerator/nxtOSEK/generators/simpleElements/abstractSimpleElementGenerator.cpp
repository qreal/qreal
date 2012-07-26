#include "abstractsimpleelementgenerator.h"

#include "../abstractElementGenerator.h"

#include "../simpleElementFactory.h"

using namespace robots::generator;

AbstractSimpleElementGenerator::AbstractSimpleElementGenerator()
{
}

AbstractSimpleElementGenerator::~AbstractSimpleElementGenerator()
{
}

QList<SmartLine> AbstractSimpleElementGenerator::convertedCode(NxtOSEKRobotGenerator *nxtGen
		, const qReal::Id elementId, const qReal::Id logicElementId)
{
	QString const elementType = elementId.element();
	AbstractSimpleElementGenerator *currentSimpleElement = SimpleElementFactory::generator(elementType);

	QList<SmartLine> result = currentSimpleElement->convertElementIntoDirectCommand(nxtGen, elementId, logicElementId);
	return result;
}

QList<QString> AbstractSimpleElementGenerator::portsToEngineNames(QString const &portsProperty) const
{
	QList<QString> result;

	if (portsProperty.contains("A")) {
		result.append("NXT_PORT_A");
	}
	if (portsProperty.contains("B")) {
		result.append("NXT_PORT_B");
	}
	if (portsProperty.contains("C")) {
		result.append("NXT_PORT_C");
	}

	return result;
}

QString AbstractSimpleElementGenerator::transformSign(QString const &sign) const
{
	if (sign == "меньше") {
		return "<";
	} else if (sign == "больше"){
		return ">";
	} else if (sign == "не меньше") {
		return ">=";
	} else if (sign == "не больше") {
		return "<=";
	} else if (sign == "равно") {
		return "==";
	}
	return "";
}
