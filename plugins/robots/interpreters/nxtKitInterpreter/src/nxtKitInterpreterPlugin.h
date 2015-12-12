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

#include <kitBase/kitPluginInterface.h>
#include <twoDModel/engine/twoDModelControlInterface.h>
#include <nxtKit/blocks/nxtBlocksFactory.h>

#include "nxtAdditionalPreferences.h"
#include "robotModel/real/usbRealRobotModel.h"
#include "robotModel/real/bluetoothRealRobotModel.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace nxt {

class NxtKitInterpreterPlugin : public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxt.NxtKitInterpreterPlugin")

public:
	NxtKitInterpreterPlugin();
	~NxtKitInterpreterPlugin() override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;

	QString kitId() const override;
	QString friendlyKitName() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<qReal::ActionInfo> customActions() override;  // Transfers ownership of embedded QActions
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QString defaultSettingsFile() const override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;
	QWidget *quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model) override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

private:
	QWidget *produceBluetoothPortConfigurer();  // Transfers ownership

	robotModel::real::UsbRealRobotModel mUsbRealRobotModel;
	robotModel::real::BluetoothRealRobotModel mBluetoothRealRobotModel;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModel;

	/// @todo Use shared pointers instead of this sh~.
	blocks::NxtBlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership
	bool mOwnsBlocksFactory = true;

	NxtAdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	QString mCurrentlySelectedModelName;
};

}
