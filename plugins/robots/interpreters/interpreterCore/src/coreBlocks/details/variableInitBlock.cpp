#include "variableInitBlock.h"

using namespace interpreterCore::coreBlocks::details;

void VariableInitBlock::run()
{
	int position = 0;
	QString const code = stringProperty("variable") + " = " + stringProperty("value");
	mParser->functionBlockParseProcess(code, position, mGraphicalId);
	if (mParser->hasErrors()) {
		mParser->deselect();
		emit failure();
	} else {
		emit done(mNextBlockId);
	}
}
