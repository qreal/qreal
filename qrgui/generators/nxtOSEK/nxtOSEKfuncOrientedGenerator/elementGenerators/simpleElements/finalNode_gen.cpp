#include "finalNode_gen.h"

using namespace qReal;
using namespace generators;
using namespace nxtOSEKgenerator;

void FinalNode_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"halt();", //TODO: use smth, what halts program
				mElementId));
}
