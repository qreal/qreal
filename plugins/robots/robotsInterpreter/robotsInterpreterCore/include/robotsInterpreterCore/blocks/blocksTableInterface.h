#pragma once

#include <robotsInterpreterCore/blocks/block.h>

namespace robotsInterpreterCore {
namespace blocks {

class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}
	virtual Block *block(qReal::Id const &element) = 0;
};

}
}
