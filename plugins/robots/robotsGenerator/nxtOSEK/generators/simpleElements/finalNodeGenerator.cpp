#include "finalNodeGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

FinalNodeGenerator::FinalNodeGenerator()
{
}

QList<SmartLine> FinalNodeGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(nxtGen)
	Q_UNUSED(logicElementId)

	QList<SmartLine> result;
	result.append(SmartLine("TerminateTask();", elementId));

	return result;
}
