#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for switching block.
class SwitchBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	void run() override;

private:
	bool initNextBlocks() override;

	QMap<QString, qReal::Id> mBranches;
	qReal::Id mDefaultBranch;
};

}
}
}
