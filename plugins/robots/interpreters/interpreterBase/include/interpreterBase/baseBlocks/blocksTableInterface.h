#pragma once

#include <interpreterBase/baseBlocks/blockInterface.h>

namespace interpreterBase {
namespace baseBlocks {

/// Centralized blocks storage. Provides mapping from block ids to objects that implement logic of block.
class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}

	/// Returns object that implement logic of block by given id.
	virtual BlockInterface *block(qReal::Id const &element) = 0;
};

}
}
