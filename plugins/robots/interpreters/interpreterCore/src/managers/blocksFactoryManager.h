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

	QList<interpreterBase::blocksBase::BlocksFactoryInterface *> factoriesFor(
			interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) override;

private:
	QMap<interpreterBase::robotModel::RobotModelInterface const *
			, interpreterBase::blocksBase::BlocksFactoryInterface *> mFactories;

};

}
