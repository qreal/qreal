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
		mNxtGen->currentGenerator()->generatedStringSet() << QList<SmartLine>();
		mNxtGen->currentGenerator()->elementToStringListNumbers()[mElementId.toString()]
				<< mNxtGen->currentGenerator()->generatedStringSet().size() - 1;
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

void AbstractElementGenerator::error(QString const &message)
{
	mNxtGen->errorReporter().addError(message, mElementId);
}

QString AbstractElementGenerator::sensorExpression(int port) const
{
	QString const portString = QString::number(port);
	qReal::interpreters::robots::enums::sensorType::SensorTypeEnum const portValue =
			mNxtGen->portValue(port);
	switch (portValue) {
	case qReal::interpreters::robots::enums::sensorType::colorRed:
	case qReal::interpreters::robots::enums::sensorType::colorGreen:
	case qReal::interpreters::robots::enums::sensorType::colorBlue:
	case qReal::interpreters::robots::enums::sensorType::colorFull:
	case qReal::interpreters::robots::enums::sensorType::colorNone:
		return "ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + portString + ") * 100 / 1023";
	case qReal::interpreters::robots::enums::sensorType::sonar:
		return "ecrobot_get_sonar_sensor(NXT_PORT_S" + portString + ")";
	case qReal::interpreters::robots::enums::sensorType::light:
		return "ecrobot_get_light_sensor(NXT_PORT_S" + portString + ") * 100 / 1023";
	case qReal::interpreters::robots::enums::sensorType::sound:
		return "ecrobot_get_sound_sensor(NXT_PORT_S" + portString + ") * 100 / 1023";
	case qReal::interpreters::robots::enums::sensorType::gyroscope:
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

	if (mNxtGen->currentGenerator()->elementToStringListNumbers().contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection

		qReal::Id loopElement = mElementId;
		if (!mNxtGen->currentGenerator()->previousLoopElements().empty()) {
			loopElement = mNxtGen->currentGenerator()->previousLoopElementsPop();
		}

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mNxtGen
				, loopElement, *mNxtGen->api());

		int num = mNxtGen->currentGenerator()->elementToStringListNumbersPop(loopElement.toString());
		QList<SmartLine> set = mNxtGen->currentGenerator()->generatedStringSet()[num]
				+ loopElementGen->addLoopCodeInPrefixForm();
		mNxtGen->currentGenerator()->setGeneratedStringSet(num, set);
		mNxtGen->currentGenerator()->generatedStringSet() << loopElementGen->addLoopCodeInPostfixForm();

		return true;
	}

	//in case element has more than 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	return nextElementsGeneration();
}
