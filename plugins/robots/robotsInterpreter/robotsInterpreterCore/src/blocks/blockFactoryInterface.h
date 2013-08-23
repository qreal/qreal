#pragma once

#include <robotsInterpreterCore/blocks/blockInterface.h>
#include <qrkernel/ids.h>

namespace robotsInterpreterCore {
namespace blocks {

class BlocksFactoryInterface
{
public:
	virtual ~BlocksFactoryInterface() {}
	virtual BlockInterface *block(qReal::Id const &element) = 0;
	virtual qReal::IdList providedBlocks() const = 0;

};

}
}
