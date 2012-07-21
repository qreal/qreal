#include "beepGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

BeepGenerator::BeepGenerator()
{
}

QList<SmartLine> BeepGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine("ecrobot_sound_tone(1000, 100, " + nxtGen->api()->stringProperty(logicElementId
			, "Volume") + ");", elementId));

	return result;
}
