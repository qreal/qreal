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

#include "interpreterCore/managers/graphicsWatcherManager.h"

#include "interpreterCore/managers/robotModelManager.h"

using namespace interpreterCore;
using namespace utils;
using namespace kitBase::robotModel;

GraphicsWatcherManager::GraphicsWatcherManager(const qrtext::DebuggerInterface &parser
		, RobotModelManager &robotManager, QObject *parent)
	: QObject(parent)
	, mWatcher(new sensorsGraph::SensorsGraph(parser))
	, mRobotManager(robotManager)
{
	mWatcher->setStartStopButtonsVisible(false);
	connect(&mRobotManager, &RobotModelManagerInterface::robotModelChanged, this, [=](RobotModelInterface &model) {
		updateSensorsList(model.robotId());
	});
}

QWidget *GraphicsWatcherManager::widget()
{
	return mWatcher;
}

void GraphicsWatcherManager::forceStart()
{
	mWatcher->startJob();
}

void GraphicsWatcherManager::forceStop()
{
	mWatcher->stopJob();
}

void GraphicsWatcherManager::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &sensor, Reason reason)
{
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	Q_UNUSED(reason)

	if (robotModel == mRobotManager.model().robotId()) {
		updateSensorsList(robotModel);
	}
}

void GraphicsWatcherManager::updateSensorsList(const QString &currentRobotModel)
{
	mWatcher->clearTrackingObjects();
	int index = 0;
	for (const PortInfo &port : configuredPorts(currentRobotModel)) {
		const DeviceInfo device = currentConfiguration(currentRobotModel, port);
		/// @todo It must depend on port, port must return its variable
		const QString variableName = port.reservedVariable();
		if (!device.isNull() && !variableName.isEmpty()) {
			mWatcher->addTrackingObject(index, variableName, QString("%1: %2").arg(port.name()
					, device.friendlyName()));
			++index;
		}
	}
}
