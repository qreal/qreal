#include "kitBase/blocksBase/robotsBlock.h"

using namespace kitBase;
using namespace blocksBase;
using namespace qReal;

RobotsBlock::RobotsBlock()
	: mRobotModelManager(nullptr)
{
}

void RobotsBlock::init(const Id &graphicalId
		, const GraphicalModelAssistInterface &graphicalModelApi
		, const LogicalModelAssistInterface &logicalModelApi
		, ErrorReporterInterface * const errorReporter
		, const robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	init(graphicalId, graphicalModelApi, logicalModelApi, errorReporter, textLanguageToolbox);
	mRobotModelManager = &robotModelManager;
}

void RobotsBlock::init(const qReal::Id &graphicalId
		, const qReal::GraphicalModelAssistInterface &graphicalModelApi
		, const qReal::LogicalModelAssistInterface &logicalModelApi
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

kitBase::robotModel::RobotModelInterface &RobotsBlock::model()
{
	return mRobotModelManager->model();
}
