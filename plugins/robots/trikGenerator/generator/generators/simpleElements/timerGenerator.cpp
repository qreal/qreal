#include "timerGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

TimerGenerator::TimerGenerator()
{
}

QList<SmartLine> TimerGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *trikGenerator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine("brick.wait(" + trikGenerator->intExpression(logicElementId
			, "Delay") + ")", elementId));

	return result;
}
