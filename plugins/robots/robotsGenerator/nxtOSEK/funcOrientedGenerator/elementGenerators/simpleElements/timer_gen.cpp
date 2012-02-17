#include "timer_gen.h"

using namespace qReal;
using namespace robots::generator;

void Timer_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"systick_wait_ms(" + mNxtGen->mApi->stringProperty(mElementId, "Delay") + ");",
				mElementId));
}
