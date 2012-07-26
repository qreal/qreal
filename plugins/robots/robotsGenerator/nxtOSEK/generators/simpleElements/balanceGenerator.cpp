#include "balanceGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

BalanceGenerator::BalanceGenerator()
{
}

QList<SmartLine> BalanceGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const forward = nxtGen->api()->stringProperty(logicElementId, "forward");
	QString const turn = nxtGen->api()->stringProperty(logicElementId, "turn");
	QString const gyroPort = nxtGen->api()->stringProperty(logicElementId, "port");
	QString const gyroValue = nxtGen->api()->stringProperty(logicElementId, "gyroOffset");
	QString const firstMotorValue = nxtGen->api()->stringProperty(logicElementId, "port1");
	QString const secondMotorValue = nxtGen->api()->stringProperty(logicElementId, "port2");
	QString const outPowerValue1 = nxtGen->api()->stringProperty(logicElementId, "pwm1");
	QString const outPowerValue2 = nxtGen->api()->stringProperty(logicElementId, "pwm2");
	result << (SmartLine(QString("balance_control("), elementId)) << (SmartLine(QString("(F32)") + forward + ", "
			, elementId)) << (SmartLine(QString("(F32)") + turn + ", ", elementId))
			<< (SmartLine(QString("(F32)ecrobot_get_gyro_sensor(NXT_PORT_S") + gyroPort + "), ", elementId))
			<< (SmartLine(QString("(F32)") + gyroValue + ", ", elementId))
			<< (SmartLine(QString("(F32)nxt_motor_get_count(NXT_PORT_") + firstMotorValue + "), ", elementId))
			<< (SmartLine(QString("(F32)nxt_motor_get_count(NXT_PORT_") + secondMotorValue + "), ", elementId))
			<< (SmartLine(QString("(F32)ecrobot_get_battery_voltage(), "), elementId))
			<< (SmartLine(QString("&") + outPowerValue1 + ", ", elementId))
			<< (SmartLine(QString("&") + outPowerValue2 + ");", elementId));

	nxtGen->mBalancerIsActivated = true;

	return result;
}
