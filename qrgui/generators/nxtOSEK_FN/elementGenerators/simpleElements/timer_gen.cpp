#include "timer_gen.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace generators;

void Timer_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"systick_wait_ms(" + mNxtGen->mApi->stringProperty(mElementId, "Delay") + ");",
				mElementId));
}
