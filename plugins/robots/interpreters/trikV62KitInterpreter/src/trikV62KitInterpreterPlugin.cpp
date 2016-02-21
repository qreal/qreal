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
	: TrikKitInterpreterPluginBase(), mStart(tr("Start QTS"), nullptr), mStop(tr("Stop QTS"), nullptr)
{
	const auto realRobotModel = new robotModel::real::RealRobotModel(kitIdString, "trikV62KitRobot");
	const auto twoDRobotModel = new robotModel::twoD::TwoDRobotModel(*realRobotModel);
	const auto blocksFactory = new blocks::TrikV62BlocksFactory();

	initKitInterpreterPluginBase(realRobotModel, twoDRobotModel, blocksFactory);

	connectDevicesConfigurationProvider(devicesConfigurationProvider()); // ...

	connect(&mStart, &QAction::triggered, this, &testStart);
	connect(&mStop, &QAction::triggered, this, &testStop);
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
	return { qReal::ActionInfo(&mStart, "file", "tools"), qReal::ActionInfo(&mStop, "file", "tools") };
}

void TrikV62KitInterpreterPlugin::testStart()
{
//	auto model = defaultRobotModel();
//	robotModel::parts::TrikDisplay * const display =
//			kitBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(*model, "DisplayPort");
//	if (display) display->drawSmile(false);
//	qDebug(defaultRobotModel()->name().toStdString().c_str());
	/// todo: bad
//	qtsInterpreter()->interpretStringScript("brick.stop()");

	robotModel::twoD::TwoDRobotModel * model = dynamic_cast<robotModel::twoD::TwoDRobotModel*>(defaultRobotModel());
	model->stopRobot();
	const QString modelName = model->robotId();

	for (const kitBase::robotModel::PortInfo &port : model->configurablePorts()) {
		const kitBase::robotModel::DeviceInfo deviceInfo = currentConfiguration(modelName, port);
		model->configureDevice(port, deviceInfo);
	}

	model->applyConfiguration();

	qtsInterpreter()->init();

	qtsInterpreter()->interpretStringScript(
	            "brick.display().setBackground(\"white\");"
	            "brick.display().redraw();"
	            "while(true){brick.smile();script.wait(1000);brick.sadSmile();script.wait(1000)}"
	            );
	//qtsInterpreter()->interpretStringScript("brick.smile();script.wait(2000);brick.sadSmile();script.wait(2000);script.quit()");

	emit started();
}

void TrikV62KitInterpreterPlugin::testStop()
{
	//qtsInterpreter()->interpretStringScript("script.quit()");
	qtsInterpreter()->abort();
	defaultRobotModel()->stopRobot();
	emit stopped(qReal::interpretation::StopReason::userStop);

}
