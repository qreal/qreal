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
#include <kitBase/robotModel/robotModelUtils.h>

#include "robotModel/real/trikV62RealRobotModel.h"
#include "robotModel/twoD/trikV62TwoDRobotModel.h"

#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h"

const QString kitIdString = "trikV62Kit";

TrikV62KitInterpreterPlugin::TrikV62KitInterpreterPlugin()
	: TrikKitInterpreterPluginBase(), mAction(tr("TEST QTS"), nullptr)
{
	const auto realRobotModel = new robotModel::real::RealRobotModel(kitIdString, "trikV62KitRobot");
	const auto twoDRobotModel = new robotModel::twoD::TwoDRobotModel(*realRobotModel);
	const auto blocksFactory = new blocks::TrikV62BlocksFactory();

	initKitInterpreterPluginBase(realRobotModel, twoDRobotModel, blocksFactory);
	connect(&mAction, &QAction::triggered, this, &testSmile);
}

QString TrikV62KitInterpreterPlugin::kitId() const
{
	return kitIdString;
}

QString TrikV62KitInterpreterPlugin::friendlyKitName() const
{
	return tr("TRIK (model-2015)");
}

QList<qReal::ActionInfo> TrikV62KitInterpreterPlugin::customActions()
{
	return { qReal::ActionInfo(&mAction, "file", "tools") };
}

void TrikV62KitInterpreterPlugin::testSmile()
{
//	auto model = defaultRobotModel();
//	robotModel::parts::TrikDisplay * const display =
//			kitBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(*model, "DisplayPort");
//	if (display) display->drawSmile(false);
//	qDebug(defaultRobotModel()->name().toStdString().c_str());
	/// todo: bad
//	defaultRobotModel()->stopRobot();
//	devicesConfigurationProvider();
//	const QString modelName = defaultRobotModel()->robotId();

//	for (const kitBase::robotModel::PortInfo &port : defaultRobotModel()->configurablePorts()) {
//	//	const kitBase::robotModel::DeviceInfo deviceInfo = currentConfiguration(modelName, port);
//	//	defaultRobotModel()->configureDevice(port, deviceInfo);
//	}
	defaultRobotModel()->applyConfiguration();
	//defaultRobotModel()->timeline().start(
	qtsInterpreter()->interpretStringScript("while(true){brick.smile();script.wait(1000);}");
}
