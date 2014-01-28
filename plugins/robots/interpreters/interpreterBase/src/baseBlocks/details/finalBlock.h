#pragma once

#include <interpreterBase/baseBlocks/block.h>

namespace interpreterBase {
namespace baseBlocks {
namespace details {

class FinalBlock : public Block
{
	Q_OBJECT

public:
	virtual void run();

private:
	virtual bool initNextBlocks();
};

}
}
}
