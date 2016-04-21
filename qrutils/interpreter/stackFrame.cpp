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

StackFrame::StackFrame(BlockInterface *block, const QList<QPair<QString, QVariant>> &properties)
	: mBlock(block)
	, mProperties(properties)
{
}

BlockInterface *StackFrame::block() const
{
	return mBlock;
}

const QList<QPair<QString, QVariant>> &StackFrame::properties() const
{
	return mProperties;
}
