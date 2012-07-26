#include "initialBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

InitialBlockGenerator::InitialBlockGenerator()
{
}

QList<SmartLine> InitialBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QList<SmartLine> initCode;
	QList<SmartLine> terminateCode;
	int const numberOfPorts = 4;
	for (int i = 1; i <= numberOfPorts; i++) {
		QString const curPort = "port_" + QString::number(i);
		QByteArray portValue = nxtGen->api()->stringProperty(logicElementId, curPort).toUtf8();
		QString const initEcrobotColorSensorPortS = "ecrobot_init_nxtcolorsensor(NXT_PORT_S";
		if (portValue == "Ультразвуковой сенсор") {
			initCode.append(SmartLine("ecrobot_init_sonar_sensor(NXT_PORT_S" + QString::number(i) + ");"
					, elementId));
			terminateCode.append(SmartLine("ecrobot_term_sonar_sensor(NXT_PORT_S" + QString::number(i) + ");"
					, elementId));
		} else if (portValue == "Сенсор цвета (все цвета)") {
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_LIGHTSENSOR_WHITE);", elementId));
			terminateCode.append(SmartLine("ecrobot_term_nxtcolorsensor(NXT_PORT_S"
			+ QString::number(i) + ");", elementId));
		} else if (portValue == "Сенсор цвета (красный)") {
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ",  NXT_LIGHTSENSOR_RED);", elementId));
			terminateCode.append(SmartLine("ecrobot_term_nxtcolorsensor(NXT_PORT_S"
			+ QString::number(i) + ");", elementId));
		} else if (portValue == "Сенсор цвета (зеленый)") {
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_LIGHTSENSOR_GREEN);", elementId));
			terminateCode.append(SmartLine("ecrobot_term_nxtcolorsensor(NXT_PORT_S"
			+ QString::number(i) + ");", elementId));
		} else if (portValue == "Сенсор цвета (синий)") {
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_LIGHTSENSOR_BLUE);", elementId));
			terminateCode.append(SmartLine("ecrobot_term_nxtcolorsensor(NXT_PORT_S"
			+ QString::number(i) + ");", elementId));
		} else if (portValue == "Сенсор цвета (пассивный)") {
			initCode.append(SmartLine(initEcrobotColorSensorPortS + QString::number(i)
				+ ", NXT_COLORSENSOR);", elementId));
			terminateCode.append(SmartLine("ecrobot_term_nxtcolorsensor(NXT_PORT_S"
			+ QString::number(i) + ");", elementId));
		}
	}
	nxtGen->initCode().append(initCode);
	nxtGen->terminateCode().append(terminateCode);

	return result;
}
