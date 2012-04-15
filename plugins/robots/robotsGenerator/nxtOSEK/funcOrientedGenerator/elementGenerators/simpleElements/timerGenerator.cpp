#include "timerGenerator.h"

using namespace qReal;
using namespace robots::generator;

void TimerGenerator::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"systick_wait_ms(" + mNxtGen->mApi->stringProperty(mElementId, "Delay") + ");",
				mElementId));
}
