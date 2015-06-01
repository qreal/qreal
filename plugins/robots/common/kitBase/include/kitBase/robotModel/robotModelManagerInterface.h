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

#include "robotModelInterface.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {

/// Provides reference to currently selected robot model and notification when it changes. All references are guaranteed
/// to be valid through all lifetime of a plugin, but particular model or devices can be in uninitialized state.
class ROBOTS_KIT_BASE_EXPORT RobotModelManagerInterface : public QObject
{
	Q_OBJECT

public:
	~RobotModelManagerInterface() override {}

	/// Returns current selected robot model.
	virtual RobotModelInterface &model() const = 0;

signals:
	/// Emitted every time when user selected other robot model.
	/// @param model - newly selected robot model.
	void robotModelChanged(RobotModelInterface &model);

	/// Signal from underlying model, emitted when QReal physically connects to robot. If there is no need for
	/// connection, it is emitted immediately. Signal correctly reconnects to a model when it is changed, so client
	/// code shall connect to this signal instead of model.
	/// @param success - true, if connected successfully.
	/// @param errorString - if connection failed, contains string to show to user.
	void connected(bool success, const QString &errorString);

	/// Signal from underlying model, emitted when robot is disconnected. Signal correctly reconnects to a model
	/// when it is changed, so client code shall connect to this signal instead of model.
	void disconnected();

	/// Signal from underlying model, emitted when model is finished configuring devices. Signal correctly reconnects
	/// to a model when it is changed, so client code shall connect to this signal instead of model.
	void allDevicesConfigured();
};

}
}
