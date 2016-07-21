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

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"

namespace ev3 {

namespace robotModel {
class Ev3GeneratorRobotModel;
}

namespace blocks {
class Ev3BlocksFactory;
}

namespace communication {
class Ev3RobotCommunicationThread;
}

/// A base class for every generator from the EV3 kit.
class ROBOTS_EV3_GENERATOR_BASE_EXPORT Ev3GeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	Ev3GeneratorPluginBase(const QString &usbRobotName, const QString &usbRobotFriendlyName, int usbPriority
			, const QString &bluetoothRobotName, const QString &bluetoothRobotFriendlyName, int bluetoothPriority);

	~Ev3GeneratorPluginBase() override;

	QString kitId() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

protected:
	void onCurrentRobotModelChanged(kitBase::robotModel::RobotModelInterface &model) override;
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;
	communication::Ev3RobotCommunicationThread *currentCommunicator();

private:
	QScopedPointer<robotModel::Ev3GeneratorRobotModel> mUsbRobotModel;
	QScopedPointer<robotModel::Ev3GeneratorRobotModel> mBluetoothRobotModel;
	blocks::Ev3BlocksFactory *mBlocksFactory;  // Transfers ownership
};

}

