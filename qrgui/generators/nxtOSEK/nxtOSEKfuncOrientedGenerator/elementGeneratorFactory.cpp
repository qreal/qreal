#include "elementGeneratorFactory.h"
#include "elementGenerators/allGenerators.h"

using namespace qReal;
using namespace generators;
using namespace nxtOSEKgenerator; 

AbstractElementGenerator* ElementGeneratorFactory::generator(NxtOSEKfuncOrientedGenerator* gen, Id elementId)
{
	//if (elementId.element() == "")
	//	return (gen, elementId);

	if (elementId.element() == "Beep")
		return new Beep_gen(gen, elementId);
	if (elementId.element() == "EnginesBackward")
		return new EnginesBackward_gen(gen, elementId);
	if (elementId.element() == "EnginesForward")
		return new EnginesForward_gen(gen, elementId);
	if (elementId.element() == "EnginesStop")
		return new EnginesStop_gen(gen, elementId);
	if (elementId.element() == "FinalNode")
		return new FinalNode_gen(gen, elementId);
	if (elementId.element() == "InitialBlock")
		return new InitialBlock_gen(gen, elementId);
	if (elementId.element() == "InitialNode")
		return new InitialNode_gen(gen, elementId);
	if (elementId.element() == "NullificationEncoder")
		return new NullificationEncoder_gen(gen, elementId);
	if (elementId.element() == "PlayTone")
		return new PlayTone_gen(gen, elementId);
	if (elementId.element() == "Timer")
		return new Timer_gen(gen, elementId);
	if (elementId.element() == "WaitForColor")
		return new WaitForColor_gen(gen, elementId);
	if (elementId.element() == "WaitForColorIntensity")
		return new WaitForColorIntensity_gen(gen, elementId);
	if (elementId.element() == "WaitForEncoder")
		return new WaitForEncoder_gen(gen, elementId);
	if (elementId.element() == "WaitForSonarDistance")
		return new WaitForSonarDistance_gen(gen, elementId);
	if (elementId.element() == "WaitForTouchSensor")
		return new WaitForTouchSensor_gen(gen, elementId);
	if (elementId.element() == "Function")
		return new Function_gen(gen, elementId);

	if (elementId.element() == "Loop")
		return new Loop_gen(gen, elementId);
	if (elementId.element() == "IfBlock")
		return new IfBlock_gen(gen, elementId);

	return 0;
}
