/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
