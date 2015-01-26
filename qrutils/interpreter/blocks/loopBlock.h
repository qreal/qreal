#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for loop block. When running it for the first time computes
/// and remembers iterations count and executes one branch for this number of times.
class QRUTILS_EXPORT LoopBlock : public Block
{
	Q_OBJECT

public:
	LoopBlock();

	void run() override;

private:
	bool initNextBlocks() override;

	/// Iterations count computation occures here.
	void additionalInit();

	qReal::Id mIterationStartBlockId;
	int mIterations;
	bool mFirstRun;
};

}
}
}
