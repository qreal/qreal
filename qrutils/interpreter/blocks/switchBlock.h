#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for switching block.
class QRUTILS_EXPORT SwitchBlock : public Block
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
