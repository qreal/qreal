#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "blocksFactoryManagerInterface.h"

namespace interpreterCore {

class BlocksFactoryManager : public BlocksFactoryManagerInterface
{
public:
	~BlocksFactoryManager();

	void addFactory(interpreterBase::blocksBase::BlocksFactoryInterface * const factory) override;

	interpreterBase::blocksBase::BlockInterface *block(qReal::Id const &element) override;

	qReal::IdList providedBlocks() const override;

private:
	QList<interpreterBase::blocksBase::BlocksFactoryInterface *> mFactories;

};

}
