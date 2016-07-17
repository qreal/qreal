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

#include "trikV6QtsGeneratorPlugin.h"

#include <trikKit/blocks/trikV6BlocksFactory.h>
#include <trikGeneratorBase/robotModel/trikV6GeneratorRobotModel.h>

using namespace trik;
using namespace trik::qts;
using namespace kitBase::robotModel;

TrikV6QtsGeneratorPlugin::TrikV6QtsGeneratorPlugin()
	: TrikQtsGeneratorPluginBase(new robotModel::TrikV6GeneratorRobotModel(
					"trikKit"
					, "trikKitRobot"
					, "TrikQtsGeneratorRobotModel"
					, tr("Generation (Java Script)")
					, 9 /* After 2D model */)
			, new blocks::TrikV6BlocksFactory()
			, {":/trikV6Qts/templates", ":/trikQts/templates"})
	, mModel(static_cast<robotModel::TrikV6GeneratorRobotModel *>(robotModels().first()))
{
}

QString TrikV6QtsGeneratorPlugin::kitId() const
{
	return "trikKit";
}

void TrikV6QtsGeneratorPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	TrikQtsGeneratorPluginBase::init(configurator);
	addShellDevice(*mModel);
}
