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
