#pragma once

#include <qrutils/interpreter/blockInterface.h>

namespace qReal {
namespace interpretation {

/// Centralized blocks storage. Provides mapping from block ids to objects that implement logic of block.
/// @todo Seems to be unneeded here.
class BlocksTableInterface
{
public:
	virtual ~BlocksTableInterface() {}

	/// Returns object that implement logic of block by given id.
	virtual BlockInterface *block(qReal::Id const &element) = 0;

	/// Clears blocks table.
	virtual void clear() = 0;

	/// Sets "failure" state for all blocks, which allows to abort program execution: if block is failed, it will not
	/// invoke next block.
	virtual void setFailure() = 0;
};

}
}
