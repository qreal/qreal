#pragma once

#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/blocksBase/blocksFactoryInterface.h>

namespace interpreterCore {

class BlocksFactoryManagerInterface
{
public:
	/// Binds the given factory to the given robot model. If robotModel is null then the
	/// factory is considered to be binded to all robot models.
	/// Takes ownership.
	virtual void addFactory(interpreterBase::blocksBase::BlocksFactoryInterface * const factory
			, interpreterBase::robotModel::RobotModelInterface const *robotModel = nullptr) = 0;

	/// Produces the block of the given type for the given robot model.
	/// If such type of blocks is disabled or unknown then nullptr is returned.
	/// Transfers ownership.
	virtual interpreterBase::blocksBase::BlockInterface *block(
			qReal::Id const &element
			, interpreterBase::robotModel::RobotModelInterface const &robotModel) = 0;

	/// Returns a list of blocks that factories can produce for the given robot model
	virtual QSet<qReal::Id> enabledBlocks(
			interpreterBase::robotModel::RobotModelInterface const &robotModel) const = 0;

	/// Returns a list of blocks that can be produced by any factory of the same kit as given robot model.
	/// Needed to grey-out such blocks in palette.
	virtual QSet<qReal::Id> visibleBlocks(
			interpreterBase::robotModel::RobotModelInterface const &robotModel) const = 0;

};

}
