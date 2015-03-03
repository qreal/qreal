#pragma once

#include <interpreterBase/blocksBase/blocksFactoryInterface.h>

namespace interpreterCore {

/// Interface for registering block factories and providing blocks to interpreter using registered factories.
class BlocksFactoryManagerInterface
{
public:
	virtual ~BlocksFactoryManagerInterface() {}

	/// Binds the given factory to the given robot model. If robotModel is null then the
	/// factory is considered to be binded to all robot models.
	/// Takes ownership.
	virtual void addFactory(interpreterBase::blocksBase::BlocksFactoryInterface * const factory
			, const interpreterBase::robotModel::RobotModelInterface *robotModel = nullptr) = 0;

	/// Produces the block of the given type for the given robot model.
	/// If such type of blocks is disabled or unknown then nullptr is returned.
	/// Transfers ownership.
	virtual qReal::interpretation::BlockInterface *block(
			const qReal::Id &element
			, const interpreterBase::robotModel::RobotModelInterface &robotModel) = 0;

	/// Returns a set of blocks that factories can produce for the given robot model
	virtual QSet<qReal::Id> enabledBlocks(
			const interpreterBase::robotModel::RobotModelInterface &robotModel) const = 0;

	/// Returns a set of blocks that can be produced by any factory of the same kit as given robot model.
	/// Needed to grey-out such blocks in palette.
	virtual QSet<qReal::Id> visibleBlocks(
			const interpreterBase::robotModel::RobotModelInterface &robotModel) const = 0;

	/// Returns a set of blocks that are produced by the common for all robot models factories.
	virtual QSet<qReal::Id> commonBlocks() const = 0;
};

}
