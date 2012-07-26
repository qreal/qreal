#include "nullificationEncoderGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

NullificationEncoderGenerator::NullificationEncoderGenerator()
{
}

QList<SmartLine> NullificationEncoderGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString const port = nxtGen->api()->stringProperty(logicElementId, "Port");
	result.append(SmartLine("nxt_motor_set_count(NXT_PORT_" + port + ", 0);", elementId));

	return result;
}
