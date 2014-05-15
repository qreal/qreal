#include "functionBlock.h"

using namespace interpreterCore::coreBlocks::details;

void FunctionBlock::run()
{
	evaluate("Body");
	emit done(mNextBlockId);
}

QVariant FunctionBlock::evaluate(QString const &propertyName)
{
	int position = 0;
	mParser->functionBlockParseProcess(stringProperty(propertyName), position, mGraphicalId);
	if (mParser->hasErrors()) {
		mParser->deselect();
		emit failure();
	}

	return 0;
}
