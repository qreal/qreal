#include "enginesForwardBlock.h"

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

EnginesForwardBlock::EnginesForwardBlock(RobotModelInterface &robotModel)
	: EngineCommandBlock(robotModel)
{
}

void EnginesForwardBlock::run()
{
	int const power = evaluate("Power").toInt();

	/// @todo Makes sense only for NXT.
	bool const breakMode = stringProperty("Mode") != QString::fromUtf8("скользить");

	for (robotParts::Motor * const motor : parsePorts()) {
		motor->on(power, breakMode);
	}

	emit done(mNextBlockId);
}
