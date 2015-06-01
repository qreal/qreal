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

#include "nullKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

using namespace nullKitInterpreter;

NullKitInterpreterPlugin::NullKitInterpreterPlugin()
	: mRobotModel(kitId())
{
}

QString NullKitInterpreterPlugin::kitId() const
{
	return "nullKit";
}

QString NullKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Empty Kit");
}

QList<kitBase::robotModel::RobotModelInterface *> NullKitInterpreterPlugin::robotModels()
{
	return QList<kitBase::robotModel::RobotModelInterface *>() << &mRobotModel;
}

kitBase::blocksBase::BlocksFactoryInterface *NullKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return nullptr;
}

QList<kitBase::AdditionalPreferences *> NullKitInterpreterPlugin::settingsWidgets()
{
	return {nullptr};
}

QList<qReal::ActionInfo> NullKitInterpreterPlugin::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> NullKitInterpreterPlugin::hotKeyActions()
{
	return {};
}

QIcon NullKitInterpreterPlugin::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
