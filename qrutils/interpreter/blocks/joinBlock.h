#pragma once

#include <qrutils/interpreter/block.h>

namespace qReal {
namespace interpretation {
namespace blocks {

class QRUTILS_EXPORT JoinBlock : public Block
{
public:
	void run() override;

private:
	bool initNextBlocks() override;

	QString mSurvivingId;
};

}
}
}
