#include "timerGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

TimerGenerator::TimerGenerator()
{
}

QList<SmartLine_old> TimerGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;
	result.append(SmartLine_old("systick_wait_ms(" + nxtGen->intExpression(logicElementId
			, "Delay") + ");", elementId));

	return result;
}
