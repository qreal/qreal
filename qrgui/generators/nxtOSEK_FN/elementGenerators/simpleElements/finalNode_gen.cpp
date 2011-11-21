#include "finalNode_gen.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace generators;

void FinalNode_gen::generateBodyWithoutNextElementCall()
{
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"halt();", //TODO: use smth, what halts program
				mElementId));
}
