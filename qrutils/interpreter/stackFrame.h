#pragma once

#include "blockInterface.h"

namespace qReal {
namespace interpretation {

class StackFrame
{
public:
	StackFrame();
	StackFrame(BlockInterface *block);
	StackFrame(BlockInterface *block, const QList<QPair<QString, QVariant>> &properties);
	BlockInterface *block() const;
	QList<QPair<QString, QVariant>> const &properties() const;

private:
	BlockInterface *mBlock;
	//<property name, property value>
	QList<QPair<QString, QVariant>> mProperties;
};
}
}
