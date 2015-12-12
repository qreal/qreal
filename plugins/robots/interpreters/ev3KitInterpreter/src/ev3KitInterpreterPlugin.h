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
#include <ev3Kit/blocks/ev3BlocksFactory.h>

#include "ev3AdditionalPreferences.h"
#include "robotModel/real/usbRealRobotModel.h"
#include "robotModel/real/bluetoothRealRobotModel.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace ev3 {

class Ev3KitInterpreterPlugin : public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "ev3.Ev3KitInterpreterPlugin")

public:
	Ev3KitInterpreterPlugin();
	~Ev3KitInterpreterPlugin() override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;

	QString kitId() const override;
	QString friendlyKitName() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<qReal::ActionInfo> customActions() override;  // Transfers ownership of embedded QActions
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;
	QWidget *quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model) override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

private:
	QWidget *produceBluetoothPortConfigurer();  // Transfers ownership

	robotModel::real::UsbRealRobotModel mUsbRealRobotModel;
	robotModel::real::BluetoothRealRobotModel mBluetoothRealRobotModel;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModel;

	blocks::Ev3BlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership
	bool mOwnsBlocksFactory = true;

	Ev3AdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	QString mCurrentlySelectedModelName;
};

}
