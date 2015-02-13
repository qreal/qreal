#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <interpreterBase/robotModel/robotModelInterface.h>
#include "blocksFactoryManagerInterface.h"

namespace interpreterCore {

/// Implementation of blocks factory manager that can provide information about blocks supported by given kit and given
/// model, and create blocks as required by interpreter.
class BlocksFactoryManager : public BlocksFactoryManagerInterface
{
public:
	~BlocksFactoryManager() override;

	void addFactory(interpreterBase::blocksBase::BlocksFactoryInterface * const factory
			, const interpreterBase::robotModel::RobotModelInterface *robotModel = nullptr) override;

	qReal::interpretation::BlockInterface *block(const qReal::Id &element
			, const interpreterBase::robotModel::RobotModelInterface &robotModel) override;

	QSet<qReal::Id> enabledBlocks(const interpreterBase::robotModel::RobotModelInterface &robotModel) const override;

	QSet<qReal::Id> visibleBlocks(const interpreterBase::robotModel::RobotModelInterface &robotModel) const override;

	QSet<qReal::Id> commonBlocks() const override;

private:
	/// Provides a list of factories that can create blocks for given robot model (common factory that creates blocks
	/// for all models, and model-specific factories).
	QList<interpreterBase::blocksBase::BlocksFactoryInterface *> factoriesFor(
			const interpreterBase::robotModel::RobotModelInterface &robotModel) const;

	/// Maps robot model to a set of block factories that can provide blocks for it.
	// Has ownership over factories.
	// Does not have ownership over robot models.
	QMap<const interpreterBase::robotModel::RobotModelInterface *
			, interpreterBase::blocksBase::BlocksFactoryInterface *> mFactories;

};

}
