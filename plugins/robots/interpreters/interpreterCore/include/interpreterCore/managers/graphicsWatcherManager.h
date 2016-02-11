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

#include <QtCore/QObject>

#include <qrutils/graphicsWatcher/sensorsGraph.h>
#include <kitBase/devicesConfigurationProvider.h>

#include <qrtext/debuggerInterface.h>

namespace interpreterCore {

class RobotModelManager;

/// Incapsulates inner operations on managing graphics watcher in the dock window.
class GraphicsWatcherManager : public QObject, public kitBase::DevicesConfigurationProvider
{
public:
	/// Constructor.
	/// @param parser - blocks parser, used to show values of variables on graphs.
	/// @param robotManager - object that controls currently selected robot model.
	/// @param parent - parent of this widget in Qt widget hierarchy.
	GraphicsWatcherManager(const qrtext::DebuggerInterface &parser
			, RobotModelManager &robotManager
			, QObject *parent = 0);

	/// Returns the graphics watcher widget itself for placing it into dock.
	/// Note that if this method will not be called, nobody will delete a widget, which will result in memleak.
	QWidget *widget();  // Transfers ownership

public slots:
	/// Starts graphics watcher`s job even if user stopped it himself.
	void forceStart();

	/// Stops graphics watcher`s job even if user started it himself.
	void forceStop();

private:
	void onDeviceConfigurationChanged(const QString &robotModel
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &sensor
			, Reason reason) override;

	void updateSensorsList(const QString &currentRobotModel);

	utils::sensorsGraph::SensorsGraph *mWatcher;  // Doesn`t have ownership
	RobotModelManager &mRobotManager;
};

}
