#include "elementGeneratorFactory.h"
#include "elementGenerators/allGenerators.h"

using namespace qReal;
using namespace robots::generator;

AbstractElementGenerator* ElementGeneratorFactory::generator(FuncOrientedGenerator* gen, Id elementId)
{
	if (elementId.element() == "Beep") {
		return new BeepGenerator(gen, elementId);

	} else if (elementId.element() == "EnginesBackward") {
		return new EnginesBackwardGenerator(gen, elementId);

	} else if (elementId.element() == "EnginesForward") {
		return new EnginesForwardGenerator(gen, elementId);

	} else if (elementId.element() == "EnginesStop") {
		return new EnginesStopGenerator(gen, elementId);

	} else if (elementId.element() == "FinalNode") {
		return new FinalNodeGenerator(gen, elementId);

	} else if (elementId.element() == "InitialBlock") {
		return new InitialBlockGenerator(gen, elementId);

	} else if (elementId.element() == "InitialNode") {
		return new InitialNodeGenerator(gen, elementId);

	} else if (elementId.element() == "NullificationEncoder") {
		return new NullificationEncoderGenerator(gen, elementId);

	} else if (elementId.element() == "PlayTone") {
		return new PlayToneGenerator(gen, elementId);

	} else if (elementId.element() == "Timer") {
		return new TimerGenerator(gen, elementId);

	} else if (elementId.element() == "WaitForColor") {
		return new WaitForColorGenerator(gen, elementId);

	} else if (elementId.element() == "WaitForColorIntensity") {
		return new WaitForColorIntensityGenerator(gen, elementId);

	} else if (elementId.element() == "WaitForEncoder") {
		return new WaitForEncoderGenerator(gen, elementId);

	} else if (elementId.element() == "WaitForSonarDistance") {
		return new WaitForSonarDistanceGenerator(gen, elementId);

	} else if (elementId.element() == "WaitForTouchSensor") {
		return new WaitForTouchSensorGenerator(gen, elementId);

	} else if (elementId.element() == "Function") {
		return new FunctionGenerator(gen, elementId);

	} else if (elementId.element() == "Loop") {
		return new LoopGenerator(gen, elementId);

	} else if (elementId.element() == "IfBlock") {
		return new IfBlockGenerator(gen, elementId);
	}

	return 0;
}
