#include "readBtArrayBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

ReadBtArrayBlockGenerator::ReadBtArrayBlockGenerator()
{
}

QString ReadBtArrayBlockGenerator::generateReadCommand(NxtOSEKRobotGenerator *nxtGen, qReal::Id const &logicElementId)
{
	QString name = nxtGen->api()->stringProperty(logicElementId, "ArrayName").toUtf8();
	//maybe doesn't work
	return "memcpy(" + name + ", bt_read_buf, sizeof(" + name + "));";
}
