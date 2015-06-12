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

#include "twoDModel/robotModel/twoDRobotModel.h"
#include "src/engine/view/nullTwoDModelDisplayWidget.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
class NullTwoDModelDisplayWidget;
}

namespace robotModel {

class NullTwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	NullTwoDRobotModel(const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	QList<kitBase::robotModel::PortInfo> configurablePorts() const override;
	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;
	QString robotImage() const;
	kitBase::robotModel::PortInfo defaultLeftWheelPort() const;
	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;

	engine::TwoDModelDisplayWidget *displayWidget() const;

private:
	engine::TwoDModelDisplayWidget *mDisplayWidget;
};

}
}
