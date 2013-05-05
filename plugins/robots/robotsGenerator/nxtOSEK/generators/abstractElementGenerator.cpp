#include "abstractElementGenerator.h"

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

void AbstractElementGenerator::replaceSensorAndEncoderVariables(QString &target)
{
	target.replace("Sensor1", replaceSensorVariables(mNxtGen->portValue(1)) + "1)");
	target.replace("Sensor2", replaceSensorVariables(mNxtGen->portValue(2)) + "2)");
	target.replace("Sensor3", replaceSensorVariables(mNxtGen->portValue(3)) + "3)");
	target.replace("Sensor4", replaceSensorVariables(mNxtGen->portValue(4)) + "4)");

	target.replace("EncoderA", replaceEncoderVariables() + "A)");
	target.replace("EncoderB", replaceEncoderVariables() + "B)");
	target.replace("EncoderC", replaceEncoderVariables() + "C)");
}

QString AbstractElementGenerator::replaceSensorVariables(qReal::interpreters::robots::sensorType::SensorTypeEnum portValue) const
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

QString AbstractElementGenerator::replaceEncoderVariables() const
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
