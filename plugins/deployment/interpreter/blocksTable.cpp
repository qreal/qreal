#include "blocksTable.h"

#include <qrutils/interpreter/blocks/initialBlock.h>
#include <qrutils/interpreter/blocks/finalBlock.h>

#include <qrutils/interpreter/blocks/functionBlock.h>
#include <qrutils/interpreter/blocks/variableInitBlock.h>

#include <qrutils/interpreter/blocks/loopBlock.h>
#include <qrutils/interpreter/blocks/forkBlock.h>
#include <qrutils/interpreter/blocks/ifBlock.h>
#include <qrutils/interpreter/blocks/switchBlock.h>
#include <qrutils/interpreter/blocks/commentBlock.h>
#include <qrutils/interpreter/blocks/subprogramBlock.h>

#include "blocks/incrementVersionInSourceCodeBlock.h"
#include "blocks/helpWithChangelogBlock.h"
#include "blocks/buildOnVirtualMachineBlock.h"
#include "blocks/uploadToGoogleDriveBlock.h"
#include "blocks/updateDownloadsCounterBlock.h"
#include "blocks/uploadToGoogleSiteBlock.h"

using namespace deployment;

BlocksTable::BlocksTable(qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguageToolbox
		, ShellWidget *shellWidget)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mErrorReporter(errorReporter)
	, mTextLanguageToolbox(textLanguageToolbox)
	, mShellWidget(shellWidget)
{
}

qReal::interpretation::Block *BlocksTable::produceRawBlock(qReal::Id const &element)
{
	if (elementMetatypeIs(element, "InitialNode")) {
		return new qReal::interpretation::blocks::InitialBlock();
	} else if (elementMetatypeIs(element, "FinalNode")) {
		return new qReal::interpretation::blocks::FinalBlock();
	} else if (elementMetatypeIs(element, "CommentBlock")) {
		return new qReal::interpretation::blocks::CommentBlock;
	} else if (elementMetatypeIs(element, "IfBlock")) {
		return new qReal::interpretation::blocks::IfBlock();
	} else if (elementMetatypeIs(element, "SwitchBlock")) {
		return new qReal::interpretation::blocks::SwitchBlock();
	} else if (elementMetatypeIs(element, "Loop")) {
		return new qReal::interpretation::blocks::LoopBlock();
	} else if (elementMetatypeIs(element, "Fork")) {
		return new qReal::interpretation::blocks::ForkBlock();
	} else if (elementMetatypeIs(element, "Subprogram")) {
		return new qReal::interpretation::blocks::SubprogramBlock();
	} else if (elementMetatypeIs(element, "Function")) {
		return new qReal::interpretation::blocks::FunctionBlock();
	} else if (elementMetatypeIs(element, "VariableInit")) {
		return new qReal::interpretation::blocks::VariableInitBlock();
	} else if (elementMetatypeIs(element, "IncrementVersionInSourceCode")) {
		return new blocks::IncrementVersionInSourceCodeBlock(mShellWidget);
	} else if (elementMetatypeIs(element, "HelpWithChangelog")) {
		return new blocks::HelpWithChangelogBlock(mShellWidget);
	} else if (elementMetatypeIs(element, "BuildOnVirtualMachine")) {
		return new blocks::BuildOnVirtualMachineBlock(mShellWidget);
	} else if (elementMetatypeIs(element, "UploadToGoogleDrive")) {
		return new blocks::UploadToGoogleDriveBlock(mShellWidget);
	} else if (elementMetatypeIs(element, "UpdateDownloadsCounter")) {
		return new blocks::UpdateDownloadsCounterBlock(mShellWidget);
	} else if (elementMetatypeIs(element, "UploadToGoogleSite")) {
		return new blocks::UploadToGoogleSiteBlock(mShellWidget);
	}

	return nullptr;
}

qReal::interpretation::BlockInterface *BlocksTable::produceBlock(qReal::Id const &element)
{
	qReal::interpretation::Block * const block = produceRawBlock(element);
	block->init(element, mGraphicalModelApi, mLogicalModelApi, &mErrorReporter, mTextLanguageToolbox);
	return block;
}

bool BlocksTable::elementMetatypeIs(qReal::Id const &element, QString const &metatype)
{
	return element.type() == qReal::Id("DeploymentEditor", "DeploymentDiagram", metatype);
}
