#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for final block. Stops current thread execution.
class QRUTILS_EXPORT FinalBlock : public Block
{
	Q_OBJECT

public:
	void run() override;

private:
	bool initNextBlocks() override;
};

}
}
}
