/* Copyright 2017 QReal Research Group
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

#include "pioneerKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

using namespace pioneerKitInterpreter;

PioneerKitInterpreterPlugin::PioneerKitInterpreterPlugin()
	: mRobotModel(kitId())
{
}

QString PioneerKitInterpreterPlugin::kitId() const
{
	return "pioneerKit";
}

QString PioneerKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Pioneer Kit");
}

QList<kitBase::robotModel::RobotModelInterface *> PioneerKitInterpreterPlugin::robotModels()
{
	return QList<kitBase::robotModel::RobotModelInterface *>() << &mRobotModel;
}

kitBase::blocksBase::BlocksFactoryInterface *PioneerKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return &mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> PioneerKitInterpreterPlugin::settingsWidgets()
{
	return {nullptr};
}

QList<qReal::ActionInfo> PioneerKitInterpreterPlugin::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> PioneerKitInterpreterPlugin::hotKeyActions()
{
	return {};
}

QIcon PioneerKitInterpreterPlugin::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
