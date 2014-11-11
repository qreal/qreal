#pragma once

#include <interpreterBase/blocksBase/blockInterface.h>

namespace interpreterBase {
namespace blocksBase {

/// Centralized blocks storage. Provides mapping from block ids to objects that implement logic of block.
/// @todo Seems to be unneeded here.
class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}

	/// Returns object that implement logic of block by given id.
	virtual BlockInterface *block(qReal::Id const &element) = 0;
};

}
}
