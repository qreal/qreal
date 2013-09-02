#include "waitForAccelerometerBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace qReal::robots::generators::simple;

WaitForAccelerometerBlockGenerator::WaitForAccelerometerBlockGenerator()
{
}

void WaitForAccelerometerBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const &elementId)
{
	QString const initCode = "ecrobot_init_accel_sensor(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_term_accel_sensor(" + port + ");";
		nxtGen->initCode().append(SmartLine_old(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine_old(terminateCode, elementId));
	}
}

QList<SmartLine_old> WaitForAccelerometerBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

	QString const acceleration = nxtGen->api()->stringProperty(logicElementId, "Acceleration");
	QString const axis = nxtGen->api()->stringProperty(logicElementId, "AccelerationAxis");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString condition;
	if (axis == "x") {
		condition.arg(condition, "buf[0]");
	} else if (axis == "y") {
		condition.arg(condition, "buf[1]");
	} else if (axis == "z") {
		condition.arg(condition, "buf[2]");
	} else {
		condition.arg(condition, "sqrt(buf[0]*buf[0]+buf[1]*buf[1]+buf[2]*buf[2])");
	}
	condition += inequalitySign + " " + acceleration;;

	result.append(SmartLine_old("int[3] buf;",elementId));
	result.append(SmartLine_old("while (!(ecrobot_get_accel_sensor(" + port + "" + ", buf[3]"
			+ ") " + " && " + condition + ")) {", elementId));
	result.append(SmartLine_old("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);
	return result;
}
