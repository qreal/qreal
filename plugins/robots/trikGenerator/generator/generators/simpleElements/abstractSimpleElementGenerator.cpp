#include "abstractSimpleElementGenerator.h"

#include "../abstractElementGenerator.h"

#include "../simpleElementFactory.h"

using namespace robots::trikGenerator;

AbstractSimpleElementGenerator::AbstractSimpleElementGenerator()
{
}

AbstractSimpleElementGenerator::~AbstractSimpleElementGenerator()
{
}

QList<SmartLine> AbstractSimpleElementGenerator::convertedCode(TrikRobotGenerator *generator
		, const qReal::Id elementId, const qReal::Id logicElementId)
{
	QString const elementType = elementId.element();
	AbstractSimpleElementGenerator *currentSimpleElement = SimpleElementFactory::generator(elementType);

	QList<SmartLine> result = currentSimpleElement->convertElementIntoDirectCommand(generator, elementId, logicElementId);
	return result;
}

QList<QString> AbstractSimpleElementGenerator::portsToEngineNames(QString const &portsProperty) const
{
	QList<QString> result;

	if (portsProperty.contains("A")) {
		result.append("0");
	}
	if (portsProperty.contains("B")) {
		result.append("1");
	}
	if (portsProperty.contains("C")) {
		result.append("2");
	}
	if (portsProperty.contains("D")) {
		result.append("3");
	}
	if (portsProperty.contains("E")) {
		result.append("4");
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
