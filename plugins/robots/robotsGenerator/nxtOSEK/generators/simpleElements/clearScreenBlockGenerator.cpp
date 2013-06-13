#include "clearScreenBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

ClearScreenBlockGenerator::ClearScreenBlockGenerator()
{
}

QList<SmartLine> ClearScreenBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	if(nxtGen->imageGenerator().bmpFilesNumber()) {
		result.append(SmartLine("memset(lcd, 0x00, sizeof(lcd));", elementId));
		result.append(SmartLine("memset(lcd_copy, 0x00, sizeof(lcd));", elementId));
	}
	result.append(SmartLine("display_clear(1);", elementId));

	return result;
}
