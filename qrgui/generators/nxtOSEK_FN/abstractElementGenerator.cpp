#include "abstractElementGenerator.h"

using namespace qReal;
using namespace generators; 

int AbstractElementGenerator::mElementCounter = 0;

QString AbstractElementGenerator::nextMethodName()
{
	QString result = "func" + mElementId.element() + QString::number(mElementCounter);
	mElementCounter++;
	return result;
}

void AbstractElementGenerator::generate()
{
	mNxtGen->mAlreadyGeneratedElements.insert(mElementId);

	QString methodName = mNxtGen->mIdToMethodNameMap[mElementId.toString()];

	mNxtGen->mGeneratedStrings.append(SmartLine("void " + methodName + "()", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId, SmartLine::increase));
	
	generateMethodBody();

	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId, SmartLine::decrease));
	mNxtGen->mGeneratedStrings.append(SmartLine("", mElementId)); //just for space between methods
}

QList<QString> AbstractElementGenerator::portsToEngineNames(QString const &portsProperty)
{
	QList<QString> result;

	//port {A, B, C} -> NXT_PORT_{A, B, C}
	if (portsProperty.contains("A"))
		result.append("NXT_PORT_A");
	if (portsProperty.contains("B"))
		result.append("NXT_PORT_B");
	if (portsProperty.contains("C"))
		result.append("NXT_PORT_C");

	return result;
}

QString AbstractElementGenerator::transformSign(QString const &sign)
{
	if (sign == "меньше")
		return "<";
	else if (sign == "больше")
		return ">";
	else if (sign == "не меньше")
		return ">=";
	else if (sign == "не больше")
		return "<=";
	else if (sign == "равно")
		return "==";
	return "";
}
