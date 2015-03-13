#pragma once

#include <qrutils/interpreter/block.h>

namespace trik {
namespace blocks {
namespace details {

class LineDetectorToVariableBlock : public qReal::interpretation::Block
{
public:
	LineDetectorToVariableBlock();

	void run();
};

}
}
}
