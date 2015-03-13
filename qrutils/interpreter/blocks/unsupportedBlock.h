#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Represents a block of a type which is unsupported by an interpreter.
/// Interpretation of this block will cause an error.
class QRUTILS_EXPORT UnsupportedBlock : public Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
