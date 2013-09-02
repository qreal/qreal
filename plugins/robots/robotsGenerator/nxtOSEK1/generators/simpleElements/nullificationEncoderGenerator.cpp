#include "nullificationEncoderGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

NullificationEncoderGenerator::NullificationEncoderGenerator()
{
}

QList<SmartLine_old> NullificationEncoderGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;
	QString const port = nxtGen->api()->stringProperty(logicElementId, "Ports");
	result.append(SmartLine_old("nxt_motor_set_count(NXT_PORT_" + port + ", 0);", elementId));

	return result;
}
