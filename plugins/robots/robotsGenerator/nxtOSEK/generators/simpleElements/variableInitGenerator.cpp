#include "variableInitGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

VariableInitGenerator::VariableInitGenerator()
{
}

QList<SmartLine_old> VariableInitGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;

	QString const variableName = nxtGen->api()->stringProperty(logicElementId, "variable");
	QString const variableValue = nxtGen->intExpression(logicElementId, "value");
	result.append(SmartLine_old(variableName + " = " + variableValue + "; \n", elementId));

	return result;
}
