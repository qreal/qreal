/* Copyright 2016 CyberTech Labs Ltd.
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

#include <qrgui/controller/commands/abstractCommand.h>

#include "createRemoveSensorImplementation.h"

namespace twoDModel {
namespace commands {

/// Creates element in 2D model world.
class CreateSensorCommand : public qReal::commands::AbstractCommand
{
public:
	/// Creates and configures sensor for the \a robotModel on port \a port of the type \a device.
	/// Sensor will be placed at \a position and oriented at \a direction degrees.
	CreateSensorCommand(model::SensorsConfiguration &configurator
						, const QString &robotModel
						, const kitBase::robotModel::PortInfo &port
						, const kitBase::robotModel::DeviceInfo &device
						, const QPointF &position
						, const qreal direction);

protected:
	bool execute() override;
	bool restoreState() override;

private:
	CreateRemoveSensorImplementation mImpl;
};

}
}
