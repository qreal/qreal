#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {


/// Interpreter`s implementation for subprogram block.
/// Starts explosed diagram interpretation using stepInto(Id) signal.
class QRUTILS_EXPORT SubprogramBlock : public Block
{
	Q_OBJECT

public:
	SubprogramBlock();

	void run() override;
	void finishedSteppingInto() override;
};

}
}
}
