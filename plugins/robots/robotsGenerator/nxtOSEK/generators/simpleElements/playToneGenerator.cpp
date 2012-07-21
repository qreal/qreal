#include "playToneGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

PlayToneGenerator::PlayToneGenerator()
{
}

QList<SmartLine> PlayToneGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine("ecrobot_sound_tone(" + nxtGen->api()->stringProperty(logicElementId, "Frequency") + ", "
			 + nxtGen->api()->stringProperty(logicElementId, "Duration") + ", "
			 + nxtGen->api()->stringProperty(logicElementId, "Volume") + ");", elementId));

	return result;
}
