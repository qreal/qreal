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

#include "nullTwoDRobotModel.h"

using namespace twoDModel::robotModel;

NullTwoDRobotModel::NullTwoDRobotModel(const QString &robotId)
	: TwoDRobotModel(robotId)
	, mDisplayWidget(new twoDModel::engine::NullTwoDModelDisplayWidget)
{
}

QString NullTwoDRobotModel::name() const
{
	return "NullTwoDRobotModel";
}
QString NullTwoDRobotModel::friendlyName() const
{
	return "";
}

QList<kitBase::robotModel::PortInfo> NullTwoDRobotModel::configurablePorts() const
{
	return QList<kitBase::robotModel::PortInfo>();
}
QList<kitBase::robotModel::DeviceInfo> NullTwoDRobotModel::convertibleBases() const
{
	return QList<kitBase::robotModel::DeviceInfo>();
}

QString NullTwoDRobotModel::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

kitBase::robotModel::PortInfo NullTwoDRobotModel::defaultLeftWheelPort() const
{
	return kitBase::robotModel::PortInfo();
}

kitBase::robotModel::PortInfo NullTwoDRobotModel::defaultRightWheelPort() const
{
	return kitBase::robotModel::PortInfo();
}

twoDModel::engine::TwoDModelDisplayWidget *NullTwoDRobotModel::displayWidget() const
{
	return mDisplayWidget;
}
