#include "beepGenerator.h"

using namespace qReal;
using namespace robots::generator;

void BeepGenerator::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"ecrobot_sound_tone(1000, 100, 50);", //TODO: change sound to smth
				mElementId));
}
