#include "finalNode_gen.h"

using namespace qReal;
using namespace robots::generator;

void FinalNode_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"halt();", //TODO: use smth, what halts program
				mElementId));
}
