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

#include "trikV62KitInterpreterPlugin.h"

using namespace trik;

#include <trikKit/blocks/trikV62BlocksFactory.h>

#include "robotModel/real/trikV62RealRobotModel.h"
#include "robotModel/twoD/trikV62TwoDRobotModel.h"

const QString kitIdString = "trikV62Kit";

TrikV62KitInterpreterPlugin::TrikV62KitInterpreterPlugin()
	: TrikKitInterpreterPluginBase()
{
	const auto realRobotModel = new robotModel::real::RealRobotModel(kitIdString, "trikV62KitRobot");
	const auto twoDRobotModel = new robotModel::twoD::TwoDRobotModel(*realRobotModel);
	const auto blocksFactory = new blocks::TrikV62BlocksFactory();
	initKitInterpreterPluginBase(realRobotModel, twoDRobotModel, blocksFactory);
}

QString TrikV62KitInterpreterPlugin::kitId() const
{
	return kitIdString;
}

QString TrikV62KitInterpreterPlugin::friendlyKitName() const
{
	return tr("TRIK (model-2015)");
}
