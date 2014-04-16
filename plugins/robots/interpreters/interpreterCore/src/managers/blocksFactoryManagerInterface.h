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

	/// Transfers ownership.
	virtual interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) = 0;

	virtual QList<interpreterBase::blocksBase::BlocksFactoryInterface *> factoriesFor(
			interpreterBase::robotModel::RobotModelInterface const &robotModel) const = 0;
};

}
