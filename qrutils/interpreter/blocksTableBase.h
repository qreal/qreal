#pragma once

#include <QtCore/QHash>
#include <QtCore/QScopedPointer>

#include <qrkernel/ids.h>

#include <qrutils/interpreter/blocksTableInterface.h>
#include <qrutils/interpreter/blockInterface.h>
#include <qrutils/utilsDeclSpec.h>

namespace qReal {
namespace interpretation {

/// Provides mapping from block ids to objects that implement logic of block, also creates blocks when needed, so
/// clients can simply request a block by given id, and a block table will do the rest.
/// Also supports operations that shall be performed on all blocks in a system, such as setting failure or idle flags.
/// and objects implementing logic of that blocks.
class QRUTILS_EXPORT BlocksTableBase : public BlocksTableInterface
{
public:
	/// Constructor. (c) Captain Obvious, 2008-2015
	BlocksTableBase();
	~BlocksTableBase() override;

	/// Returns the object that may be used by the interpreter to invoke the block with the given id.
	/// If met for the first time it will be produced from scratch, otherwise existing instance will be returned.
	BlockInterface *block(qReal::Id const &element) override;

	/// Clears blocks table.
	void clear() override;

	/// Sets "failure" state for all blocks, which allows to abort program execution: if block is failed, it will not
	/// invoke next block.
	void setFailure() override;

private:
	/// Must be overridded to produce the logical instance of the block itself.
	virtual BlockInterface *produceBlock(qReal::Id const &element) = 0;

	QHash<qReal::Id, BlockInterface *> mBlocks;  // Has ownership
};

}
}
