#include "beep_gen.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace generators;

void Beep_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"ecrobot_sound_tone(1000, 100, 50);", //TODO: change sound to smth
				mElementId));
}
