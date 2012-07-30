#include "simpleElementFactory.h"
#include "simpleElements/enginesGenerator.h"
#include "simpleElements/enginesStopGenerator.h"
#include "simpleElements/timerGenerator.h"
#include "simpleElements/beepGenerator.h"
#include "simpleElements/playToneGenerator.h"
#include "simpleElements/finalNodeGenerator.h"
#include "simpleElements/nullificationEncoderGenerator.h"
#include "simpleElements/initialBlockGenerator.h"
#include "simpleElements/waitForColorBlockGenerator.h"
#include "simpleElements/waitForColorIntensityBlockGenerator.h"
#include "simpleElements/balanceGenerator.h"
#include "simpleElements/waitForLightBlockGenerator.h"
#include "simpleElements/waitForSonarBlockGenerator.h"
#include "simpleElements/waitForEncoderBlockGenerator.h"
#include "simpleElements/waitForTouchSensorBlockGenerator.h"
#include "simpleElements/variableInitGenerator.h"
#include "simpleElements/balanceInitGenerator.h"
#include "simpleElements/initialNodeGenerator.h"

using namespace robots::generator;

AbstractSimpleElementGenerator* SimpleElementFactory::generator(QString const elementType)
{
	if (elementType == "EnginesForward" || elementType == "EnginesBackward") {
		return new EnginesGenerator(elementType);
	} else if (elementType == "EnginesStop") {
		return new EnginesStopGenerator();
	} else if (elementType == "Timer") {
		return new TimerGenerator();
	} else if (elementType == "Beep") {
		return new BeepGenerator();
	} else if (elementType == "PlayTone") {
		return new PlayToneGenerator();
	} else if (elementType == "FinalNode") {
		return new FinalNodeGenerator();
	} else if (elementType == "NullificationEncoder") {
		return new NullificationEncoderGenerator();
	} else if (elementType == "InitialBlock") {
		return new InitialBlockGenerator();
	} else if (elementType == "WaitForColor") {
		return new WaitForColorBlockGenerator();
	} else if (elementType == "WaitForColorIntensity") {
		return new WaitForColorIntensityBlockGenerator();
	} else if (elementType == "WaitForLight") {
		return new WaitForLightBlockGenerator();
	} else if (elementType == "WaitForTouchSensor") {
		return new WaitForTouchSensorBlockGenerator();
	} else if (elementType == "WaitForSonarDistance") {
		return new WaitForSonarBlockGenerator();
	} else if (elementType == "WaitForEncoder") {
		return new WaitForEncoderBlockGenerator();
	} else if (elementType == "Balance") {
		return new BalanceGenerator();
	} else if (elementType == "BalanceInit") {
		return new BalanceInitGenerator();
	} else if (elementType == "VariableInit") {
		return new VariableInitGenerator();
	}

	return new InitialNodeGenerator();
}
