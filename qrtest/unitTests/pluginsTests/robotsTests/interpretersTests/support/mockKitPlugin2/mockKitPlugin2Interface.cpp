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

#include "mockKitPlugin2Interface.h"

using namespace qrTest::mockKitPlugin2;

QString MockKitPlugin2Interface::kitId() const
{
	return "MockKitPlugin2";
}

QString MockKitPlugin2Interface::friendlyKitName() const
{
	return "Mock Kit Plugin 2";
}

QList<kitBase::robotModel::RobotModelInterface *> MockKitPlugin2Interface::robotModels()
{
	return QList<kitBase::robotModel::RobotModelInterface *>();
}

QList<kitBase::AdditionalPreferences *> MockKitPlugin2Interface::settingsWidgets()
{
	return {nullptr};
}

kitBase::blocksBase::BlocksFactoryInterface *MockKitPlugin2Interface::blocksFactoryFor(
		kitBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model)
	return nullptr;
}

QList<qReal::ActionInfo> MockKitPlugin2Interface::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> MockKitPlugin2Interface::hotKeyActions()
{
	return {};
}

QIcon MockKitPlugin2Interface::iconForFastSelector(
		kitBase::robotModel::RobotModelInterface const &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
