#include "initialNodeGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

InitialNodeGenerator::InitialNodeGenerator()
{
}

QList<SmartLine> InitialNodeGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED (nxtGen)
	Q_UNUSED (elementId)
	Q_UNUSED (logicElementId)

	QList<SmartLine> result;

	return result;
}
