#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for fork block. Creates new threads by calling newThread singal.
class QRUTILS_EXPORT ForkBlock : public Block
{
	Q_OBJECT

public:
	void run() override;

private:
	bool initNextBlocks() override;

	qReal::IdList mThreadStartBlocks;
};

}
}
}
