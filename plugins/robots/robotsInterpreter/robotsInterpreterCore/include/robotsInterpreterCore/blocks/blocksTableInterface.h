#pragma once

#include <robotsInterpreterCore/blocks/blockInterface.h>

namespace robotsInterpreterCore {
namespace blocks {

class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}
	virtual BlockInterface *block(qReal::Id const &element) = 0;
};

}
}
