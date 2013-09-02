#include "clearScreenBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

ClearScreenBlockGenerator::ClearScreenBlockGenerator()
{
}

QList<SmartLine_old> ClearScreenBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine_old> result;
	if(nxtGen->imageGenerator().bmpFilesCount()) {
		result.append(SmartLine_old("memset(lcd, 0x00, sizeof(lcd));", elementId));
		result.append(SmartLine_old("memset(lcd_copy, 0x00, sizeof(lcd));", elementId));
	}
	result.append(SmartLine_old("display_clear(1);", elementId));

	return result;
}
