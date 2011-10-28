#include "functionBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

FunctionBlock::FunctionBlock()
{
}

void FunctionBlock::run()
{
	evaluate("Body");
	emit done(mNextBlock);
}

QVariant FunctionBlock::evaluate(const QString &propertyName)
{
	int position = 0;
	mParser->functionBlockParseProcess(stringProperty(propertyName), position, mGraphicalId);
	if (mParser->hasErrors()) {
		mParser->deselect();
		emit failure(); /*разобраться с этой хренотой*/
	}
	return 0;

}
