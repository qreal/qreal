#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Interpreter`s implementation for initial block.
/// Does nothing except passing control to next block.
class QRUTILS_EXPORT InitialBlock : public Block
{
	Q_OBJECT

public:
	virtual void run();
};

}
}
}
