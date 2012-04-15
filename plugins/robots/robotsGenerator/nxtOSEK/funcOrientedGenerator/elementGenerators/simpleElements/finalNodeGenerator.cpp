#include "finalNodeGenerator.h"

using namespace qReal;
using namespace robots::generator;

void FinalNodeGenerator::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"halt();", //TODO: use smth, what halts program
				mElementId));
}
