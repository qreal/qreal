#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <interpreterBase/robotModel/robotModelInterface.h>
#include "blocksFactoryManagerInterface.h"

namespace interpreterCore {

class BlocksFactoryManager : public BlocksFactoryManagerInterface
{
public:
	~BlocksFactoryManager();

	void addFactory(interpreterBase::blocksBase::BlocksFactoryInterface * const factory
			, interpreterBase::robotModel::RobotModelInterface const *robotModel = nullptr) override;


	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element
			, interpreterBase::robotModel::RobotModelInterface const &robotModel) override;

	QSet<qReal::Id> enabledBlocks(interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

private:
	QList<interpreterBase::blocksBase::BlocksFactoryInterface *> factoriesFor(
			interpreterBase::robotModel::RobotModelInterface const &robotModel) const;

	QMap<interpreterBase::robotModel::RobotModelInterface const *
			, interpreterBase::blocksBase::BlocksFactoryInterface *> mFactories;

};

}
