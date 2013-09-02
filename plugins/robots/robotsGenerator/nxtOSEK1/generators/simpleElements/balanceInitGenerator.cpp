#include "balanceInitGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

BalanceInitGenerator::BalanceInitGenerator()
{
}

QList<SmartLine_old> BalanceInitGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine_old> result;

	result.append(SmartLine_old("balance_init();", elementId));
	nxtGen->activateBalancer();

	return result;
}
