#pragma once

#include <QtCore/QHash>

#include <qrkernel/ids.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/baseBlocks/blocksFactoryInterface.h>
#include <interpreterBase/baseBlocks/blocksTableInterface.h>
#include <interpreterBase/baseBlocks/blockInterface.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "robotsBlockParser.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

class BlocksTable : public interpreterBase::baseBlocks::BlocksTableInterface
{
public:
	BlocksTable(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelInterface * const robotModel
			, qReal::ErrorReporterInterface * const errorReporter
			, RobotsBlockParser * const parser
			, interpreterBase::baseBlocks::BlocksFactoryInterface * const blocksFactory
			);

	~BlocksTable();

	// Override.
	virtual interpreterBase::baseBlocks::BlockInterface *block(qReal::Id const &element);

	void clear();
	void addBlock(qReal::Id const &element, interpreterBase::baseBlocks::BlockInterface *block);
	void setFailure();
	void setIdleForBlocks();

	qReal::IdList providedBlocks() const;

private:
	QHash<qReal::Id, interpreterBase::baseBlocks::BlockInterface *> mBlocks;  // Has ownership
	interpreterBase::baseBlocks::BlocksFactoryInterface *mBlocksFactory;  // Has ownership
};

}
}
}
