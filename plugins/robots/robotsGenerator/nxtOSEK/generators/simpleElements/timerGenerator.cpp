#include "timerGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

TimerGenerator::TimerGenerator()
{
}

QList<SmartLine> TimerGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine("systick_wait_ms(" + nxtGen->api()->stringProperty(logicElementId
			, "Delay") + ");", elementId));

	return result;
}
