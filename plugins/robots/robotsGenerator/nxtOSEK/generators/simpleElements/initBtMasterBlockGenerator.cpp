#include "initBtMasterBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

InitBtMasterBlockGenerator::InitBtMasterBlockGenerator()
{
}

QList<SmartLine> InitBtMasterBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;
	QString slaveAddr = nxtGen->api()->stringProperty(logicElementId, "SlaveAddr").toUtf8();;

	nxtGen->btCommunicationGenerator().btMasterUsed();
	nxtGen->variables().setSlaveAddr(slaveAddr);

	result.append(SmartLine("WaitEvent(BtConnect);", elementId));
	result.append(SmartLine("ClearEvent(BtConnect);", elementId));

	return result;
}
