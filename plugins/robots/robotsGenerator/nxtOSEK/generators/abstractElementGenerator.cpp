#include "abstractElementGenerator.h"

#include <QtCore/QRegExp>

#include "../nxtOSEKRobotGenerator.h"
#include "../elementGeneratorFactory.h"

using namespace robots::generator;

AbstractElementGenerator::AbstractElementGenerator(NxtOSEKRobotGenerator *mainGenerator
		, qReal::Id const &elementId): mNxtGen(mainGenerator), mElementId(elementId)
{
}

AbstractElementGenerator::~AbstractElementGenerator()
{
}

void AbstractElementGenerator::createListsForIncomingConnections()
{
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mNxtGen->api()->incomingConnectedElements(mElementId).size(); i++) {
		mNxtGen->generatedStringSet() << QList<SmartLine>();
		mNxtGen->elementToStringListNumbers()[mElementId.toString()] << mNxtGen->generatedStringSet().size() - 1;
	}
}

QString AbstractElementGenerator::replaceSensorAndEncoderVariables(QString const &expression) const
{
	QString result = expression;
	for (int i = 1; i <= 4; ++i) {
		result.replace("Sensor" + QString::number(i), sensorExpression(i));
	}

	result.replace("EncoderA", encoderExpression() + "A)");
	result.replace("EncoderB", encoderExpression() + "B)");
	result.replace("EncoderC", encoderExpression() + "C)");
	return result;
}

QString AbstractElementGenerator::replaceFunctionInvocations(QString const &expression) const
{
	QString result = expression;

	QRegExp randomFunctionInvocationRegEx("random\\((.*)\\)");
	int pos = randomFunctionInvocationRegEx.indexIn(result, 0);
	while (pos != -1) {
		QString const param = randomFunctionInvocationRegEx.cap(1);
		result.replace(randomFunctionInvocationRegEx, "rand() % " + param);
		pos += randomFunctionInvocationRegEx.matchedLength();
		pos = randomFunctionInvocationRegEx.indexIn(result, pos);
	}

	return result;
}

QString AbstractElementGenerator::sensorExpression(int port) const
{
	QString const portString = QString::number(port);
	qReal::interpreters::robots::sensorType::SensorTypeEnum const portValue =
			mNxtGen->portValue(port);
	switch (portValue) {
	case qReal::interpreters::robots::sensorType::colorRed:
	case qReal::interpreters::robots::sensorType::colorGreen:
	case qReal::interpreters::robots::sensorType::colorBlue:
	case qReal::interpreters::robots::sensorType::colorFull:
	case qReal::interpreters::robots::sensorType::colorNone:
		return "ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + portString + ") * 100 / 1023";
	case qReal::interpreters::robots::sensorType::sonar:
		return "ecrobot_get_sonar_sensor(NXT_PORT_S" + portString + ")";
	case qReal::interpreters::robots::sensorType::light:
		return "ecrobot_get_light_sensor(NXT_PORT_S" + portString + ") * 100 / 1023";
	case qReal::interpreters::robots::sensorType::sound:
		return "ecrobot_get_sound_sensor(NXT_PORT_S" + portString + ") * 100 / 1023";
	case qReal::interpreters::robots::sensorType::gyroscope:
		return "ecrobot_get_gyro_sensor(NXT_PORT_S" + portString + ")";
	default:
		return "ecrobot_get_touch_sensor(NXT_PORT_S" + portString + ")";
	}
}

QString AbstractElementGenerator::encoderExpression() const
{
	return "nxt_motor_get_count(NXT_PORT_";
}

bool AbstractElementGenerator::generate()
{
	if (!preGenerationCheck()) {
		return false;
	}

	if (mNxtGen->elementToStringListNumbers().contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection

		qReal::Id loopElement = mElementId;
		if (!mNxtGen->previousLoopElements().empty()) {
			loopElement = mNxtGen->previousLoopElementsPop();
		}

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mNxtGen, loopElement, *mNxtGen->api());

		int num = mNxtGen->elementToStringListNumbersPop(loopElement.toString());
		QList<SmartLine> set = mNxtGen->generatedStringSet()[num] + loopElementGen->addLoopCodeInPrefixForm();
		mNxtGen->setGeneratedStringSet(num, set);
		mNxtGen->generatedStringSet() << loopElementGen->addLoopCodeInPostfixForm();

		return true;
	}

	//in case element has more than 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	return nextElementsGeneration();
}
