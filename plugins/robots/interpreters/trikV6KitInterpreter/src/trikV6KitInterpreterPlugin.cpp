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

#include "trikV6KitInterpreterPlugin.h"

using namespace trik;

#include <trikKit/blocks/trikV6BlocksFactory.h>

#include "robotModel/real/trikV6RealRobotModel.h"
#include "robotModel/twoD/trikV6TwoDRobotModel.h"

const QString kitIdString = "trikKit";

TrikV6KitInterpreterPlugin::TrikV6KitInterpreterPlugin()
	: TrikKitInterpreterPluginBase()
{
	const auto realRobotModel = new robotModel::real::TrikV6RealRobotModel(kitIdString, "trikKitRobot");
	const auto twoDRobotModel = new robotModel::twoD::TrikV6TwoDRobotModel(*realRobotModel);
	const auto blocksFactory = new blocks::TrikV6BlocksFactory();
	initKitInterpreterPluginBase(realRobotModel, twoDRobotModel, blocksFactory);
}

QString TrikV6KitInterpreterPlugin::kitId() const
{
	return kitIdString;
}

QString TrikV6KitInterpreterPlugin::friendlyKitName() const
{
	return tr("TRIK (model-2014)");
}

int TrikV6KitInterpreterPlugin::priority() const
{
	return 10;  // Old case is more likely will be met for now.
}
