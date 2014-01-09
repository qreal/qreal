#pragma once

#include <QtCore/QHash>

#include <qrkernel/ids.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/blocks/blocksFactoryInterface.h>
#include <interpreterBase/blocks/blocksTableInterface.h>
#include <interpreterBase/blocks/blockInterface.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "robotsBlockParser.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

class BlocksTable : public interpreterBase::blocks::BlocksTableInterface
{
public:
	BlocksTable(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelInterface * const robotModel
			, qReal::ErrorReporterInterface * const errorReporter
			, RobotsBlockParser * const parser
			, interpreterBase::blocks::BlocksFactoryInterface * const blocksFactory
			);

	~BlocksTable();

	// Override.
	virtual interpreterBase::blocks::BlockInterface *block(qReal::Id const &element);

	void clear();
	void addBlock(qReal::Id const &element, interpreterBase::blocks::BlockInterface *block);
	void setFailure();
	void setIdleForBlocks();

	qReal::IdList providedBlocks() const;

private:
	QHash<qReal::Id, interpreterBase::blocks::BlockInterface *> mBlocks;  // Has ownership
	interpreterBase::blocks::BlocksFactoryInterface *mBlocksFactory;  // Has ownership
};

}
}
}
