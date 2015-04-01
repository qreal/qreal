#pragma once

#include <qrutils/interpreter/block.h>

namespace trik {
namespace blocks {
namespace details {

/// Interpreter implementation for "Line Detector into Variable" block.
class LineDetectorToVariableBlock : public qReal::interpretation::Block
{
public:
	LineDetectorToVariableBlock();

	void run() override;
};

}
}
}
