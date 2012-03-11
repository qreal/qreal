#include "playTone_gen.h"

using namespace qReal;
using namespace robots::generator;

void PlayTone_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"ecrobot_sound_tone(" + mNxtGen->mApi->stringProperty(mElementId, "Frequency") + ", "
				+ mNxtGen->mApi->stringProperty(mElementId, "Duration") + ", 50);", //50 - volume of a sound
				mElementId));
}
