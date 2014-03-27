#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class FunctionBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	FunctionBlock();

	void run() override;

	QVariant evaluate(QString const &propertyName);
};

}
}
}
