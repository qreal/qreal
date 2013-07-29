#include "initBtSlaveBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

InitBtSlaveBlockGenerator::InitBtSlaveBlockGenerator()
{
}

QList<SmartLine> InitBtSlaveBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;

	nxtGen->btCommunicationGenerator().btSlaveUsed();

	result.append(SmartLine("WaitEvent(BtConnect);", elementId));
	result.append(SmartLine("ClearEvent(BtConnect);", elementId));

	return result;
}
