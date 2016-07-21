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

#pragma once

#include <qrutils/interpreter/block.h>

#include <kitBase/blocksBase/blocksFactoryInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

namespace kitBase {
namespace blocksBase {

/// Base class for all blocks implementations used in robots interpreter.
class ROBOTS_KIT_BASE_EXPORT RobotsBlock : public qReal::interpretation::Block
{
	Q_OBJECT

public:
	virtual QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices();

	/// Initializes this block with external components that may be used during the intepretetation process.
	/// @todo Not good.
	void init(const qReal::Id &graphicalId
			, const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, const robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);

protected:
	RobotsBlock();

	/// Reference to a robot model which is used by this block.
	robotModel::RobotModelInterface &model();

	const robotModel::RobotModelManagerInterface *mRobotModelManager;  // Doesn't have ownership.

private:
	// Just to make it private
	void init(const qReal::Id &graphicalId
			, const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);
};

}
}
