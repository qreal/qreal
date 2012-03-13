#include "initialBlockGenerator.h"

#include <QString>
#include <QByteArray>

using namespace qReal;
using namespace robots::generator;

void InitialBlockGenerator::generateBodyWithoutNextElementCall()
{
	for (int i = 1; i <= 4; i++) {
		//4 - number of ports on nxt robot
		QString curPort = "port_" + QString::number(i);
		QByteArray portValue = mNxtGen->mApi->stringProperty(mElementId, curPort).toUtf8();

		if (portValue == "Ультразвуковой сенсор") {
			mNxtGen->mGeneratedStrings.append(SmartLine(
						"ecrobot_init_sonar_sensor(NXT_PORT_S" + QString::number(i) + ")",
						mElementId));

			//in nxtOSEK there are no instructions to initiate touch sensors
			//} else if (portValue == "Сенсор нажатия (булево значение)") {
			//} else if (portValue == "Сенсор нажатия (сырое значение)") {

	} else if (portValue == "Сенсор цвета (полные цвета)") {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_WHITE)",
					mElementId));

	} else if (portValue == "Сенсор цвета (красный)") {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_RED)",
					mElementId));

	} else if (portValue == "Сенсор цвета (зеленый)") {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_GREEN)",
					mElementId));

	} else if (portValue == "Сенсор цвета (синий)") {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_BLUE)",
					mElementId));

	} else if (portValue == "Сенсор цвета (пассивный)") {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_COLORSENSOR)",
					mElementId));

	}
	}
}
