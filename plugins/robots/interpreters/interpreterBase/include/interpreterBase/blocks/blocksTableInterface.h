#pragma once

#include <interpreterBase/blocks/blockInterface.h>

namespace interpreterBase {
namespace blocks {

class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}
	virtual BlockInterface *block(qReal::Id const &element) = 0;
};

}
}
