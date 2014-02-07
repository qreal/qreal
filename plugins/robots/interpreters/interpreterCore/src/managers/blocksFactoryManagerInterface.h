#pragma once

#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/blocksBase/blocksFactoryInterface.h>

namespace interpreterCore {

class BlocksFactoryManagerInterface
{
public:
	/// Takes ownership.
	virtual void addFactory(interpreterBase::blocksBase::BlocksFactoryInterface * const factory) = 0;

	/// Transfers ownership.
	virtual interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) = 0;

	virtual qReal::IdList providedBlocks() const = 0;
};

}
