#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for conditional block.
class QRUTILS_EXPORT IfBlock : public Block
{
	Q_OBJECT

public:
	void run() override;

private:
	bool initNextBlocks() override;

	qReal::Id mElseBlockId;
};

}
}
}
