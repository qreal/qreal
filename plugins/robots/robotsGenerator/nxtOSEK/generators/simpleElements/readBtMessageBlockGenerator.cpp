#include "readBtMessageBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

ReadBtMessageBlockGenerator::ReadBtMessageBlockGenerator()
{
}

QList<SmartLine> ReadBtMessageBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;
	nxtGen->variables().hasBtRead();

	result.append(SmartLine("while(!ecrobot_read_bt_packet(bt_read_buf, 32)){systick_wait_ms(5);};", elementId));
	result.append(SmartLine(generateReadCommand(nxtGen, logicElementId), elementId));

	return result;
}
