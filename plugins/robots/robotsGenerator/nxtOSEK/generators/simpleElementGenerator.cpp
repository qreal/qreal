#include "simpleElementGenerator.h"

#include "../nxtOSEKRobotGenerator.h"
#include "../elementGeneratorFactory.h"

using namespace qReal;
using namespace robots::generator;

SimpleElementGenerator::SimpleElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

QList<QString> SimpleElementGenerator::portsToEngineNames(QString const &portsProperty)
{
	QList<QString> result;

	//port {A, B, C} -> NXT_PORT_{A, B, C}
	if (portsProperty.contains("A"))
		result.append("NXT_PORT_A");
	if (portsProperty.contains("B"))
		result.append("NXT_PORT_B");
	if (portsProperty.contains("C"))
		result.append("NXT_PORT_C");

	return result;
}

QList<SmartLine> SimpleElementGenerator::simpleCode()
{
	QList<SmartLine> result;

	qReal::Id logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	//TODO: to make "break mode" to do smth
	if (mElementId.element() == "EnginesForward") {
		QStringList cmds = mNxtGen->api()->stringProperty(logicElementId, "Power").split(";", QString::SkipEmptyParts);
		for (int i = 0; i < cmds.size() - 1; ++i)
			result.append(SmartLine(cmds.at(i) + ";", mElementId));
		foreach (QString enginePort, portsToEngineNames(mNxtGen->api()->stringProperty(logicElementId, "Ports"))) {
			result.append(SmartLine(
				"nxt_motor_set_speed(" + enginePort + ", " + cmds.last() + ", 1);",
				mElementId));
		}

	} else if (mElementId.element() == "EnginesBackward") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->api()->stringProperty(logicElementId, "Ports"))) {
			result.append(SmartLine(
				"nxt_motor_set_speed(" + enginePort + ", "
				+ "-" + mNxtGen->api()->stringProperty(logicElementId, "Power") + ", 1);",
				mElementId));
		}

	} else if (mElementId.element() == "EnginesStop") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->api()->stringProperty(logicElementId, "Ports"))) {
			result.append(SmartLine(
				"nxt_motor_set_speed(" + enginePort + ", 0, 1);",
				mElementId));
		}

	} else if (mElementId.element() == "Timer") {
		result.append(SmartLine(
				"systick_wait_ms(" + mNxtGen->api()->stringProperty(logicElementId, "Delay") + ");",
				mElementId));

	} else if (mElementId.element() == "Beep") {
		result.append(SmartLine(
				"ecrobot_sound_tone(1000, 100, " + mNxtGen->api()->stringProperty(logicElementId, "Volume") + ");", //TODO: change sound to smth
				mElementId));

	} else if (mElementId.element() == "PlayTone") {
		result.append(SmartLine(
				"ecrobot_sound_tone(" + mNxtGen->api()->stringProperty(logicElementId, "Frequency") + ", "
				 + mNxtGen->api()->stringProperty(logicElementId, "Duration") + ", "
				 + mNxtGen->api()->stringProperty(logicElementId, "Volume") + ");",
				 mElementId));

	} else if (mElementId.element() == "FinalNode") {
		result.append(SmartLine(
				"TerminateTask();",
				mElementId));

	} else if (mElementId.element() == "NullificationEncoder") {
		QString port = mNxtGen->api()->stringProperty(logicElementId, "Port");
		result.append(SmartLine(
				"nxt_motor_set_count(NXT_PORT_" + port + ", 0);",
				mElementId));

	} else if (mElementId.element() == "InitialBlock") {
		for (int i = 1; i <= 4; i++) {
			//4 - number of ports on nxt robot
			QString curPort = "port_" + QString::number(i);
			QByteArray portValue = mNxtGen->api()->stringProperty(logicElementId, curPort).toUtf8();

			switch (i) {
			case 1:
				mNxtGen->portValue1() = portValue;
				break;
			case 2:
				mNxtGen->portValue2() = portValue;
				break;
			case 3:
				mNxtGen->portValue3() = portValue;
				break;
			case 4:
				mNxtGen->portValue4() = portValue;
				break;
			}

			if (portValue == "Ультразвуковой сенсор") {
				mInitCode.append(SmartLine(
						"ecrobot_init_sonar_sensor(NXT_PORT_S" + QString::number(i) + ");",
						mElementId));
				mTerminateCode.append(SmartLine(
						"ecrobot_term_sonar_sensor(NXT_PORT_S" + QString::number(i) + ");",
						mElementId));
			//in nxtOSEK there are no instructions to initiate touch sensors
			//} else if (portValue == "Сенсор нажатия (булево значение)") {
			//} else if (portValue == "Сенсор нажатия (сырое значение)") {

			} else if (portValue == "Сенсор цвета (полные цвета)") {
					mInitCode.append(SmartLine(
							"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_WHITE);",
							mElementId));

			} else if (portValue == "Сенсор цвета (красный)") {
					mInitCode.append(SmartLine(
							"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ",  NXT_LIGHTSENSOR_RED);",
							mElementId));

			} else if (portValue == "Сенсор цвета (зеленый)") {
					mInitCode.append(SmartLine(
							"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_GREEN);",
							mElementId));

			} else if (portValue == "Сенсор цвета (синий)") {
					mInitCode.append(SmartLine(
							"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_BLUE);",
							mElementId));

			} else if (portValue == "Сенсор цвета (пассивный)") {
					mInitCode.append(SmartLine(
							"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_COLORSENSOR);",
							mElementId));
			}
		}

	} else if (mElementId.element() == "WaitForColor") {
		int port = mNxtGen->api()->stringProperty(logicElementId, "Port").toInt();
		QByteArray colorStr = mNxtGen->api()->stringProperty(logicElementId, "Color").toUtf8();

		QString colorNxtType;

		if (colorStr == "Красный") {
			colorNxtType = "NXT_COLOR_RED";

		} else if (colorStr == "Зелёный") {
			colorNxtType = "NXT_COLOR_GREEN";

		} else if (colorStr == "Синий") {
			colorNxtType = "NXT_COLOR_BLUE";

		} else if (colorStr == "Чёрный") {
			colorNxtType = "NXT_COLOR_BLACK";

		} else if (colorStr == "Жёлтый") {
			colorNxtType = "NXT_COLOR_YELLOW";

		} else if (colorStr == "Белый") {
			colorNxtType = "NXT_COLOR_WHITE";
		}

		if (!colorNxtType.isEmpty()) {
			result.append(SmartLine(
					"while (ecrobot_get_nxtcolorsensor_id(NXT_PORT_S" + QString::number(port)
						+ ") != " + colorNxtType + ")",
					mElementId));
			result.append(SmartLine("{", mElementId));
			result.append(SmartLine("}", mElementId));
		}

	} else if (mElementId.element() == "WaitForColorIntensity") {
		int port = mNxtGen->api()->stringProperty(logicElementId, "Port").toInt();
		QString intensity = mNxtGen->api()->stringProperty(logicElementId,  "Intensity");
		QString inequalitySign = transformSign(QString(mNxtGen->api()->stringProperty(logicElementId, "Sign").toUtf8()));

		QString condition = inequalitySign + " " + intensity;

		result.append(SmartLine(
				"while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + QString::number(port)
					+ ") " + condition + "))",
				mElementId));
		result.append(SmartLine("{", mElementId));
		result.append(SmartLine("}", mElementId));

	} else if (mElementId.element() == "WaitForLight"){
		int port = mNxtGen->api()->stringProperty(logicElementId, "Port").toInt();

		QString percents = mNxtGen->api()->stringProperty(logicElementId,  "Percents");
		QString inequalitySign = transformSign(QString(mNxtGen->api()->stringProperty(logicElementId, "Sign").toUtf8()));

		QString condition = inequalitySign + " " + percents;

		result.append(SmartLine(
				"while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + QString::number(port)
					+ ") " + condition + "))",
				mElementId));
		result.append(SmartLine("{", mElementId));
		result.append(SmartLine("}", mElementId));

	} else if (mElementId.element() == "WaitForTouchSensor") {
		int port = mNxtGen->api()->stringProperty(logicElementId, "Port").toInt();

		result.append(SmartLine(
				"while (!ecrobot_get_touch_sensor(NXT_PORT_S" + QString::number(port) + "))",
				mElementId));
		result.append(SmartLine("{", mElementId));
		result.append(SmartLine("}", mElementId));

	} else if (mElementId.element() == "WaitForSonarDistance") {
		int port = mNxtGen->api()->stringProperty(logicElementId, "Port").toInt();
		QString distance = mNxtGen->api()->stringProperty(logicElementId, "Distance");
		QString inequalitySign = transformSign(QString(mNxtGen->api()->stringProperty(logicElementId, "Sign").toUtf8()));
		QString condition = inequalitySign + " " + distance;

		result.append(SmartLine(
				"while (!(ecrobot_get_sonar_sensor(NXT_PORT_S" + QString::number(port) + ") " + condition + "))",
				mElementId));
		result.append(SmartLine("{", mElementId));
		result.append(SmartLine("}", mElementId));

	} else if (mElementId.element() == "WaitForEncoder") {
		QString port = mNxtGen->api()->stringProperty(logicElementId, "Port");
		QString tachoLimit = mNxtGen->api()->stringProperty(logicElementId, "TachoLimit");
		result.append(SmartLine(
				"while (nxt_motor_get_count(NXT_PORT_" + port + ") < " + tachoLimit + ")",
				mElementId));
		result.append(SmartLine("{", mElementId));
		result.append(SmartLine("}", mElementId));
	} else if (mElementId.element() == "Unknown_something") {
		result.append(SmartLine("{ int trololo = 1488; }", mElementId));
	} else if (mElementId.element() == "Balance") {
		QString const forward = mNxtGen->api()->stringProperty(logicElementId, "forward");
		QString const turn = mNxtGen->api()->stringProperty(logicElementId, "turn");
		QString const gyroPort = mNxtGen->api()->stringProperty(logicElementId, "port");
		QString const gyroValue = mNxtGen->api()->stringProperty(logicElementId, "gyroOffset");
		QString const firstMotorValue = mNxtGen->api()->stringProperty(logicElementId, "port1");
		QString const secondMotorValue = mNxtGen->api()->stringProperty(logicElementId, "port2");
		QString const outPowerValue1 = mNxtGen->api()->stringProperty(logicElementId, "pwm1");
		QString const outPowerValue2 = mNxtGen->api()->stringProperty(logicElementId, "pwm2");
		result.append(SmartLine(QString("balance_control("), mElementId));
		result.append(SmartLine(QString("(F32)") + forward + ", ", mElementId));
		result.append(SmartLine(QString("(F32)") + turn + ", ", mElementId));
		result.append(SmartLine(QString("(F32)ecrobot_get_gyro_sensor(NXT_PORT_S") + gyroPort + "), ", mElementId));
		result.append(SmartLine(QString("(F32)") + gyroValue + ", ", mElementId));
		result.append(SmartLine(QString("(F32)nxt_motor_get_count(NXT_PORT_") + firstMotorValue + "), ", mElementId));
		result.append(SmartLine(QString("(F32)nxt_motor_get_count(NXT_PORT_") + secondMotorValue + "), ", mElementId));
		result.append(SmartLine(QString("(F32)ecrobot_get_battery_voltage(), "), mElementId));
		result.append(SmartLine(QString("&") + outPowerValue1 + ", ", mElementId));
		result.append(SmartLine(QString("&") + outPowerValue2 + ");", mElementId));
	} else if (mElementId.element() == "BalanceInit") {
		result.append(SmartLine("balance_init();", mElementId));
	} else if (mElementId.element() == "VariableInit") {
		QString const variableName = mNxtGen->api()->stringProperty(logicElementId, "variable");
		QString const variableValue = mNxtGen->api()->stringProperty(logicElementId, "value");
		result.append(SmartLine(variableName + " = " + variableValue + "; \n", mElementId));
	}

	//for InitialNode returns empty list
	return result;
}

bool SimpleElementGenerator::nextElementsGeneration()
{
	IdList outgoingConnectedElements = mNxtGen->api()->outgoingConnectedElements(mElementId);
	mNxtGen->generatedStringSet() << simpleCode();
	mNxtGen->initCode().append(mInitCode);

	if (outgoingConnectedElements.size() == 1) {
		if (outgoingConnectedElements.at(0) == Id::rootId()) {
			mNxtGen->errorReporter().addError("Element " + mElementId.toString() + " has no"\
					" correct next element because its link has no end object."\
					" May be you need to connect it to diagram object.", mElementId);
			return false;
		}

		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(mNxtGen, outgoingConnectedElements.at(0), *mNxtGen->api());
		mNxtGen->previousElement() = mElementId;
		gen->generate();
		delete gen;
		return true;
	} else if ((mElementId.element() == "FinalNode") && (outgoingConnectedElements.size() == 0)) {
		return true;
	} else {
		//case of error end of diagram
		mNxtGen->errorReporter().addError(QObject::tr("There is no outgoing connected element with no final node!"), mElementId);
		return false;
	}

	return true;
}

QString SimpleElementGenerator::transformSign(QString const &sign)
{
	qDebug() << sign;

	if (sign == "меньше")
		return "<";
	else if (sign == "больше")
		return ">";
	else if (sign == "не меньше")
		return ">=";
	else if (sign == "не больше")
		return "<=";
	else if (sign == "равно")
		return "==";
	return "";
}

QList<SmartLine> SimpleElementGenerator::loopPrefixCode()
{
	QList<SmartLine> result;
	result << SmartLine("while (true) {", mElementId, SmartLine::increase);
	return result;
}

QList<SmartLine> SimpleElementGenerator::loopPostfixCode()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool SimpleElementGenerator::preGenerationCheck()
{
	IdList outgoingConnectedElements = mNxtGen->api()->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		mNxtGen->errorReporter().addError(QObject::tr("There are more than 1 outgoing connected elements with simple robot element!"), mElementId);
		return false;
	}

	return true;
}
