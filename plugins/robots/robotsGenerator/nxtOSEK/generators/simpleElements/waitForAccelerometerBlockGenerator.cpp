#include "waitForAccelerometerBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForAccelerometerBlockGenerator::WaitForAccelerometerBlockGenerator()
{
}

void WaitForAccelerometerBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const &elementId)
{
	QString const initCode = "ecrobot_init_accel_sensor(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_term_accel_sensor(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForAccelerometerBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

	QString const acceleration = nxtGen->api()->stringProperty(logicElementId, "Acceleration");
	QString const os = nxtGen->api()->stringProperty(logicElementId, "Acceleration OS");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString condition ;

	if (os == "oX"){
		condition.arg(condition, "buf[0]");
	} else if (os == "oY"){
		condition.arg(condition, "buf[1]");
	} else if (os == "oZ"){
		condition.arg(condition, "buf[2]");
	} else {
		condition.arg(condition, "sqrt(buf[0]*buf[0]+buf[1]*buf[1]+buf[2]*buf[2])");
	}
	condition += inequalitySign + " " + acceleration;;

	result.append(SmartLine("int[3] buf;",elementId));
	result.append(SmartLine("while (!(ecrobot_get_accel_sensor(" + port + "" + ", buf[3]"
			+ ") " + " && " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);
	return result;
}
