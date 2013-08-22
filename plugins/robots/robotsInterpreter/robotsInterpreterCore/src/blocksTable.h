#pragma once

#include <QtCore/QHash>

#include <qrkernel/ids.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <robotsInterpreterCore/robotModelInterface.h>

#include "robotsBlockParser.h"

namespace robotsInterpreterCore {

class Block;

class BlocksFactory;

class BlocksTable
{
public:
	BlocksTable(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, RobotModelInterface * const robotModel
			, qReal::ErrorReporterInterface * const errorReporter
			, RobotsBlockParser * const parser
			);

	~BlocksTable();
	Block *block(qReal::Id const &element);
	void clear();
	void addBlock(qReal::Id const &element, Block *block);
	void setFailure();
	void setIdleForBlocks();

	qReal::IdList commonBlocks() const;

private:
	QHash<qReal::Id, Block *> mBlocks;  // Has ownership
	BlocksFactory *mBlocksFactory;  // Has ownership
};

}
