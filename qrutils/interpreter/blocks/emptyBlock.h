#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// A block that does nothing.
class QRUTILS_EXPORT EmptyBlock : public Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
