#include "readBtVariableBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

ReadBtVariableBlockGenerator::ReadBtVariableBlockGenerator()
{
}

QString ReadBtVariableBlockGenerator::generateReadCommand(NxtOSEKRobotGenerator *nxtGen, qReal::Id const &logicElementId)
{
	QString name = nxtGen->api()->stringProperty(logicElementId, "VariableName").toUtf8();
	return "memcpy(" + name + ", bt_read_buf, sizeof(" + name + "));";
}
