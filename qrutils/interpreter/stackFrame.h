#pragma once

#include "blockInterface.h"

namespace qReal {
namespace interpretation {

class StackFrame
{
public:
	StackFrame();
	StackFrame(BlockInterface *block);
	StackFrame(BlockInterface *block, const QHash<QString, QString> &properties);
	BlockInterface *getBlock();
private:
	BlockInterface *mBlock;
	QHash<QString, QString> mProperties;
};
}
}
