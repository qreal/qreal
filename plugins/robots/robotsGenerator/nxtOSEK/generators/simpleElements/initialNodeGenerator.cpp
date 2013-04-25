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

		QString const initEcrobotColorSensorPortS = "ecrobot_init_nxtcolorsensor(NXT_PORT_S";
		SmartLine const termColorSensorCode("ecrobot_term_nxtcolorsensor(NXT_PORT_S"
				+ QString::number(i) + ");", elementId);
		SmartLine const colorSensorIsrHook("ecrobot_process_bg_nxtcolorsensor();", elementId);

		switch (portValue) {
		case qReal::interpreters::robots::sensorType::sonar:
			initCode.append(SmartLine("ecrobot_init_sonar_sensor(NXT_PORT_S" + QString::number(i) + ");"
					, elementId));
			terminateCode.append(SmartLine("ecrobot_term_sonar_sensor(NXT_PORT_S" + QString::number(i) + ");"
					, elementId));
			break;
		case qReal::interpreters::robots::sensorType::light:
			initCode.append(SmartLine("ecrobot_set_light_sensor_active(NXT_PORT_S" + QString::number(i) + ");"
					, elementId));
			terminateCode.append(SmartLine("ecrobot_set_light_sensor_inactive(NXT_PORT_S" + QString::number(i) + ");"
					, elementId));
			break;
		case qReal::interpreters::robots::sensorType::colorFull:
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_LIGHTSENSOR_WHITE);", elementId));
			terminateCode.append(termColorSensorCode);
			if (!foundColorSensor) {
				isrHooks.append(colorSensorIsrHook);
				foundColorSensor = true;
			}
			break;
		case qReal::interpreters::robots::sensorType::colorRed:
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ",  NXT_LIGHTSENSOR_RED);", elementId));
			terminateCode.append(termColorSensorCode);
			if (!foundColorSensor) {
				isrHooks.append(colorSensorIsrHook);
				foundColorSensor = true;
			}
			break;
		case qReal::interpreters::robots::sensorType::colorGreen:
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_LIGHTSENSOR_GREEN);", elementId));
			terminateCode.append(termColorSensorCode);
			if (!foundColorSensor) {
				isrHooks.append(colorSensorIsrHook);
				foundColorSensor = true;
			}
			break;
		case qReal::interpreters::robots::sensorType::colorBlue:
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_LIGHTSENSOR_BLUE);", elementId));
			terminateCode.append(termColorSensorCode);
			if (!foundColorSensor) {
				isrHooks.append(colorSensorIsrHook);
				foundColorSensor = true;
			}
			break;
		case qReal::interpreters::robots::sensorType::colorNone:
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_COLORSENSOR);", elementId));
			terminateCode.append(termColorSensorCode);
			if (!foundColorSensor) {
				isrHooks.append(colorSensorIsrHook);
				foundColorSensor = true;
			}
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
