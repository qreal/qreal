#include "initialNodeGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

InitialNodeGenerator::InitialNodeGenerator()
{
}

QList<SmartLine> InitialNodeGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;
	QList<SmartLine> initCode;
	QList<SmartLine> terminateCode;
	QList<SmartLine> isrHooks;
	bool foundColorSensor = false;

	int const numberOfPorts = 4;
	for (int i = 1; i <= numberOfPorts; ++i) {
		qReal::interpreters::robots::sensorType::SensorTypeEnum portValue = nxtGen->portValue(i);

		switch (portValue) {
		case qReal::interpreters::robots::sensorType::sonar:
			initCode.append(SmartLine("ecrobot_init_sonar_sensor(NXT_PORT_S"
					+ QString::number(i) + ");", elementId));
			terminateCode.append(SmartLine("ecrobot_term_sonar_sensor(NXT_PORT_S"
					+ QString::number(i) + ");", elementId));
			break;
		case qReal::interpreters::robots::sensorType::light:
			initCode.append(SmartLine("ecrobot_set_light_sensor_active(NXT_PORT_S"
					+ QString::number(i) + ");", elementId));
			terminateCode.append(SmartLine("ecrobot_set_light_sensor_inactive(NXT_PORT_S"
					+ QString::number(i) + ");", elementId));
			break;
		case qReal::interpreters::robots::sensorType::colorFull:
				appendColorCode(initCode, terminateCode, isrHooks
						, foundColorSensor, elementId, "NXT_LIGHTSENSOR_WHITE", i);
			break;
		case qReal::interpreters::robots::sensorType::colorRed:
			appendColorCode(initCode, terminateCode, isrHooks
					, foundColorSensor, elementId, "NXT_LIGHTSENSOR_RED", i);
			break;
		case qReal::interpreters::robots::sensorType::colorGreen:
			appendColorCode(initCode, terminateCode, isrHooks
					, foundColorSensor, elementId, "NXT_LIGHTSENSOR_GREEN", i);
			break;
		case qReal::interpreters::robots::sensorType::colorBlue:
			appendColorCode(initCode, terminateCode, isrHooks
					, foundColorSensor, elementId, "NXT_LIGHTSENSOR_BLUE", i);
			break;
		case qReal::interpreters::robots::sensorType::colorNone:
			appendColorCode(initCode, terminateCode, isrHooks
					, foundColorSensor, elementId, "NXT_COLORSENSOR", i);
			break;
		default:
			break;
		}
	}
	nxtGen->initCode().append(initCode);
	nxtGen->terminateCode().append(terminateCode);
	nxtGen->isrHooksCode().append(isrHooks);

	return result;
}

void InitialNodeGenerator::appendColorCode(QList<SmartLine> &initCode
		, QList<SmartLine> &terminateCode, QList<SmartLine> &isrHooks
		, bool &foundColorSensor, qReal::Id const &elementId
		, QString const &sensor, int port)
{
	QString const initPattern = "ecrobot_init_nxtcolorsensor(NXT_PORT_S%1, %2);";
	QString const termPattern = "ecrobot_term_nxtcolorsensor(NXT_PORT_S%1);";
	QString const portString = QString::number(port);

	SmartLine const initColorSensorCode(initPattern.arg(portString, sensor), elementId);
	SmartLine const termColorSensorCode(termPattern.arg(portString), elementId);
	SmartLine const colorSensorIsrHook("ecrobot_process_bg_nxtcolorsensor();", elementId);

	initCode.append(initColorSensorCode);
	terminateCode.append(termColorSensorCode);
	if (!foundColorSensor) {
		isrHooks.append(colorSensorIsrHook);
		foundColorSensor = true;
	}
}
