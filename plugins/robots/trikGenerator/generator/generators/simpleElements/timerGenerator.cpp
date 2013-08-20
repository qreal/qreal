#include "timerGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

TimerGenerator::TimerGenerator()
{
}

QList<SmartLine> TimerGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine("brick.wait(" + generator->intExpression(logicElementId
			, "Delay") + ");", elementId));

	return result;
}
