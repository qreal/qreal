#include <interpreterBase/blocksBase/robotsBlock.h>

using namespace interpreterBase;
using namespace blocksBase;
using namespace qReal;

RobotsBlock::RobotsBlock()
	: mRobotModelManager(nullptr)
{
}

void RobotsBlock::init(const Id &graphicalId
		, GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface const &logicalModelApi
		, ErrorReporterInterface * const errorReporter
		, robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	init(graphicalId, graphicalModelApi, logicalModelApi, errorReporter, textLanguageToolbox);
	mRobotModelManager = &robotModelManager;
}

void RobotsBlock::init(const qReal::Id &graphicalId
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::ErrorReporterInterface * const errorReporter
		, qrtext::LanguageToolboxInterface &textLanguageToolbox
		)
{
	Block::init(graphicalId, graphicalModelApi, logicalModelApi, errorReporter, textLanguageToolbox);
}

QMap<robotModel::PortInfo, robotModel::DeviceInfo> RobotsBlock::usedDevices()
{
	return {};
}

interpreterBase::robotModel::RobotModelInterface &RobotsBlock::model()
{
	return mRobotModelManager->model();
}
