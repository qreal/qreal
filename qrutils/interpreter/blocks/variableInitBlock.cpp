#include "variableInitBlock.h"

using namespace qReal::interpretation::blocks;

void VariableInitBlock::run()
{
	evalCode(stringProperty("variable") + " = " + stringProperty("value"));
	if (!errorsOccured()) {
		emit done(mNextBlockId);
	}
}
