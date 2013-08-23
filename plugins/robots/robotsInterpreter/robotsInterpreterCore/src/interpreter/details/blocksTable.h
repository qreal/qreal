#pragma once

#include <QtCore/QHash>

#include <qrkernel/ids.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <robotsInterpreterCore/blocks/blocksTableInterface.h>
#include <robotsInterpreterCore/blocks/blockInterface.h>
#include <robotsInterpreterCore/robotModel/robotModelInterface.h>

#include "robotsBlockParser.h"

namespace robotsInterpreterCore {
namespace interpreter {
namespace details {

class BlocksTable : public blocks::BlocksTableInterface
{
public:
	BlocksTable(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, robotModel::RobotModelInterface * const robotModel
			, qReal::ErrorReporterInterface * const errorReporter
			, RobotsBlockParser * const parser
			);

	~BlocksTable();

	// Override.
	virtual blocks::BlockInterface *block(qReal::Id const &element);

	void clear();
	void addBlock(qReal::Id const &element, blocks::BlockInterface *block);
	void setFailure();
	void setIdleForBlocks();

	qReal::IdList commonBlocks() const;

private:
	QHash<qReal::Id, blocks::BlockInterface *> mBlocks;  // Has ownership
	BlocksFactory *mBlocksFactory;  // Has ownership
};

}
}
}
