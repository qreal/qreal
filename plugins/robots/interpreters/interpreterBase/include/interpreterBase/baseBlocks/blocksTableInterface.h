#pragma once

#include <interpreterBase/baseBlocks/blockInterface.h>

namespace interpreterBase {
namespace baseBlocks {

class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}
	virtual BlockInterface *block(qReal::Id const &element) = 0;
};

}
}
