#include "simpleElementFactory.h"
#include "simpleElements/enginesGenerator.h"
#include "simpleElements/enginesStopGenerator.h"
#include "simpleElements/timerGenerator.h"
#include "simpleElements/beepGenerator.h"
#include "simpleElements/playToneGenerator.h"
#include "simpleElements/finalNodeGenerator.h"
#include "simpleElements/nullificationEncoderGenerator.h"
#include "simpleElements/waitForColorBlockGenerator.h"
#include "simpleElements/waitForColorIntensityBlockGenerator.h"
#include "simpleElements/balanceGenerator.h"
#include "simpleElements/drawPixelBlockGenerator.h"
#include "simpleElements/drawRectBlockGenerator.h"
#include "simpleElements/clearScreenBlockGenerator.h"
#include "simpleElements/drawLineBlockGenerator.h"
#include "simpleElements/drawCircleBlockGenerator.h"
#include "simpleElements/printTextBlockGenerator.h"
#include "simpleElements/waitForLightBlockGenerator.h"
#include "simpleElements/waitForButtonsBlockGenerator.h"
#include "simpleElements/waitForSonarBlockGenerator.h"
#include "simpleElements/waitForEncoderBlockGenerator.h"
#include "simpleElements/waitForTouchSensorBlockGenerator.h"
#include "simpleElements/waitForSoundBlockGenerator.h"
#include "simpleElements/waitForGyroscopeBlockGenerator.h"
#include "simpleElements/waitForAccelerometerBlockGenerator.h"
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
	} else if (elementType == "WaitForSound"){
		return new WaitForSoundBlockGenerator();
	} else if (elementType == "WaitForGyroscope"){
		return new WaitForGyroscopeBlockGenerator();
	} else if (elementType == "WaitForAccelerometer"){
		return new WaitForAccelerometerBlockGenerator();
	} else if (elementType == "Balance") {
		return new BalanceGenerator();
	} else if (elementType == "BalanceInit") {
		return new BalanceInitGenerator();
	} else if (elementType == "VariableInit") {
		return new VariableInitGenerator();
	} else if (elementType == "WaitForButtons") {
		return new WaitForButtonsBlockGenerator();
	} else if (elementType == "DrawPixel") {
		return new DrawPixelBlockGenerator();
	} else if (elementType == "DrawLine") {
		return new DrawLineBlockGenerator();
	} else if (elementType == "DrawCircle") {
		return new DrawCircleBlockGenerator();
	} else if (elementType == "PrintText") {
		return new PrintTextBlockGenerator();
	} else if (elementType == "DrawRect") {
		return new DrawRectBlockGenerator();
	} else if (elementType == "ClearScreen") {
		return new ClearScreenBlockGenerator();
	}

	return new InitialNodeGenerator();
}
