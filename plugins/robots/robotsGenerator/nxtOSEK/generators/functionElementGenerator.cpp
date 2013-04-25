#include "functionElementGenerator.h"

using namespace robots::generator;

FunctionElementGenerator::FunctionElementGenerator(NxtOSEKRobotGenerator *generator
		, qReal::Id const &elementId, bool const &generateToInit)
		: SimpleElementGenerator(generator, elementId), mGenerateToInit(generateToInit)
{
}

void FunctionElementGenerator::variableAnalysis(QByteArray const &code)
{
	QList<QByteArray> const funcBlocks = code.split(';');

	foreach (QByteArray const &block, funcBlocks) {
			//Only one possible place for first variable appear
		int const firstEqualSignPos = block.indexOf('=');
		if (firstEqualSignPos == -1) {
			continue;
		}

		//must be a normal variable name
		QByteArray leftPart = block.left(firstEqualSignPos);

		leftPart = leftPart.trimmed();
		QString const forbiddenLastSimbols = "+-=*/><";
		if (forbiddenLastSimbols.contains((leftPart.at(leftPart.length() - 1)))) {
			continue;
		}

		bool isVariableExisted = false;
		foreach (SmartLine const &curVariable, mNxtGen->variables()) {
			if (curVariable.text() == QString::fromUtf8(leftPart)) {
				isVariableExisted = true;
				break;
			}
		}
		if (!isVariableExisted) {
			mNxtGen->variables().append(SmartLine(QString::fromUtf8(leftPart), mElementId));
		}
	}
}

QByteArray FunctionElementGenerator::replaceSensorVariables(qReal::interpreters::robots::sensorType::SensorTypeEnum portValue) const
{
	switch (portValue) {
	case qReal::interpreters::robots::sensorType::colorRed:
	case qReal::interpreters::robots::sensorType::colorGreen:
	case qReal::interpreters::robots::sensorType::colorBlue:
	case qReal::interpreters::robots::sensorType::colorFull:
	case qReal::interpreters::robots::sensorType::colorNone:
		return "ecrobot_get_nxtcolorsensor_light(NXT_PORT_S";
	case qReal::interpreters::robots::sensorType::sonar:
		return "ecrobot_get_sonar_sensor(NXT_PORT_S";
	case qReal::interpreters::robots::sensorType::light:
		return "ecrobot_get_light_sensor(NXT_PORT_S";
	default:
		return "ecrobot_get_touch_sensor(NXT_PORT_S";
	}
}

QList<SmartLine> FunctionElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	QByteArray byteFuncCode = mNxtGen->api()->stringProperty(logicElementId, "Body").toUtf8();
	byteFuncCode.replace("Sensor1", replaceSensorVariables(mNxtGen->portValue(1)) + "1)");
	byteFuncCode.replace("Sensor2", replaceSensorVariables(mNxtGen->portValue(2)) + "2)");
	byteFuncCode.replace("Sensor3", replaceSensorVariables(mNxtGen->portValue(3)) + "3)");
	byteFuncCode.replace("Sensor4", replaceSensorVariables(mNxtGen->portValue(4)) + "4)");

	variableAnalysis(byteFuncCode);
	QString const funcCode = QString::fromUtf8(byteFuncCode);

	foreach (QString const &str, funcCode.split(';')) {
		result.append(SmartLine(str.trimmed() + ";", mElementId));
	}

	if (mGenerateToInit) {
		mNxtGen->initCode().append(result);
		result.clear();
	}

	return result;
}
