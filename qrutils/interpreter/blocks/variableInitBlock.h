#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for variable initialization block.
class QRUTILS_EXPORT VariableInitBlock : public Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
