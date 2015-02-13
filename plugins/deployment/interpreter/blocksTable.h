#pragma once

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrutils/interpreter/blocksTableBase.h>
#include <qrutils/interpreter/block.h>
#include <qrtext/languageToolboxInterface.h>

#include "shellWidget.h"

namespace deployment {

/// Implementation of blocks table for deployment plugin factory.
/// @todo: Generalize it, move to qrutils.
class BlocksTable : public qReal::interpretation::BlocksTableBase
{
public:
	BlocksTable(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			, ShellWidget *shellWidget);
private:
	qReal::interpretation::BlockInterface *produceBlock(qReal::Id const &element) override;
	qReal::interpretation::Block *produceRawBlock(qReal::Id const &element);

	bool elementMetatypeIs(qReal::Id const &element, QString const &metatype);

	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::ErrorReporterInterface &mErrorReporter;
	qrtext::LanguageToolboxInterface &mTextLanguageToolbox;
	ShellWidget *mShellWidget;
};

}
