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

#include <kitBase/kitPluginInterface.h>
#include <kitBase/robotModel/commonRobotModel.h>

namespace qrTest {
namespace mockKitPlugin2 {

class MockKitPlugin2Interface : public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin2.MockKitPlugin2Interface")

public:
	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	// Transfers ownership.
	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			kitBase::robotModel::RobotModelInterface const *model) override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QIcon iconForFastSelector(kitBase::robotModel::RobotModelInterface const &robotModel) const override;
};

}
}
