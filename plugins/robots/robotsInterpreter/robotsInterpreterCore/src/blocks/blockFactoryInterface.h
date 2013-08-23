#pragma once

#include <robotsInterpreterCore/blocks/block.h>

namespace robotsInterpreterCore {
namespace blocks {

class BlocksFactoryInterface
{
public:
	Block *block(qReal::Id const &element);
};

}
}
