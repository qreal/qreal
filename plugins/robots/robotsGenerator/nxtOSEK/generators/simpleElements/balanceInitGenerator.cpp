#include "balanceInitGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

BalanceInitGenerator::BalanceInitGenerator()
{
}

QList<SmartLine> BalanceInitGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;

	result.append(SmartLine("balance_init();", elementId));
	nxtGen->mBalancerIsActivated = true;

	return result;
}
