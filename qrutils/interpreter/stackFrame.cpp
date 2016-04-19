#include "stackFrame.h"

using namespace qReal;
using namespace interpretation;

StackFrame::StackFrame()
	: mBlock(nullptr)
{
}

StackFrame::StackFrame(BlockInterface *block)
	: mBlock(block)
{
}

StackFrame::StackFrame(BlockInterface *block, const QHash<QString, QString> &properties)
	: mBlock(block)
	, mProperties(properties)
{
}

BlockInterface *StackFrame::getBlock()
{
	return mBlock;
}
