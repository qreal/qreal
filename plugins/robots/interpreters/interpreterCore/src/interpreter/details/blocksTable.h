#pragma once

#include <QtCore/QHash>
#include <QtCore/QScopedPointer>

#include <qrkernel/ids.h>

#include <interpreterBase/baseBlocks/blocksFactoryInterface.h>
#include <interpreterBase/baseBlocks/blocksTableInterface.h>
#include <interpreterBase/baseBlocks/blockInterface.h>

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Implementation of blocks table functionality required by InterpreterBase.
/// Provides mapping from block ids to objects that implement logic of block, also creates blocks when needed, so
/// clients can simply request a block by given id, and a block table will do the rest.
/// Also supports operations that shall be performed on all blocks in a system, such as setting failure or idle flags.
/// and objects implementing logic of that blocks.
class BlocksTable : public interpreterBase::baseBlocks::BlocksTableInterface
{
public:
	/// Constructor.
	/// @param blocksFactory - a factory that is used to create new blocks when needed. BlocksTable takes ownership.
	BlocksTable(interpreterBase::baseBlocks::BlocksFactoryInterface *blocksFactory);

	~BlocksTable() override;

	interpreterBase::baseBlocks::BlockInterface *block(qReal::Id const &element) override;

	/// Clears blocks table.
	void clear();

	/// Sets "failure" state for all blocks, which allows to abort program execution: if block is failed, it will not
	/// invoke next block.
	void setFailure();

	/// Sets "idle" state for all blocks, thus preparing them for another execution.
	// TODO: possibly unneeded. It will not clean existing connections and pending signals, so it is safer to delete
	// and re-create all blocks before another run of a program.
	void setIdleForBlocks();

	/// Returns a list of blocks that can be created by this blocks table.
	qReal::IdList providedBlocks() const;

private:
	QHash<qReal::Id, interpreterBase::baseBlocks::BlockInterface *> mBlocks;  // Has ownership
	QScopedPointer<interpreterBase::baseBlocks::BlocksFactoryInterface> mBlocksFactory;
};

}
}
}
