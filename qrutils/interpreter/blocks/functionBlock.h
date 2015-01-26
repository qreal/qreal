#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for function block.
/// Uses expression parser for immediate expression`s value computation.
class QRUTILS_EXPORT FunctionBlock : public Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
