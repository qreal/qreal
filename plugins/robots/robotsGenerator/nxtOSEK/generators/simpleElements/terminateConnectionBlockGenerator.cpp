#include "terminateConnectionBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

TerminateConnectionBlockGenerator::TerminateConnectionBlockGenerator()
{
}

QList<SmartLine> TerminateConnectionBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;

	result.append(SmartLine("ecrobot_term_bt_connection();", elementId));

	return result;
}
