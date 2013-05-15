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

	result.append("display_clear(0);");

	return result;
}
