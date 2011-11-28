#include "beep_gen.h"

using namespace qReal;
using namespace generators;
using namespace nxtOSEKgenerator;

void Beep_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"ecrobot_sound_tone(1000, 100, 50);", //TODO: change sound to smth
				mElementId));
}
