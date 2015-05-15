#pragma once

#include <qrutils/interpreter/block.h>

namespace qReal {
namespace interpretation {
namespace blocks {

class QRUTILS_EXPORT JoinBlock : public Block
{
	Q_OBJECT
public:
	JoinBlock();

	void run() override;

private:
	int mIncomingTokens;
};

}
}
}
