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

#include "ev3GeneratorBase/ev3GeneratorPluginBase.h"

#include <ev3Kit/communication/bluetoothRobotCommunicationThread.h>
#include <ev3Kit/communication/usbRobotCommunicationThread.h>
#include <ev3Kit/blocks/ev3BlocksFactory.h>

#include "ev3GeneratorBase/robotModel/ev3GeneratorRobotModel.h"

using namespace ev3;

Ev3GeneratorPluginBase::Ev3GeneratorPluginBase(const QString &usbRobotName, const QString &usbRobotFriendlyName
		, int usbPriority, const QString &bluetoothRobotName
		, const QString &bluetoothRobotFriendlyName, int bluetoothPriority)
	: mUsbRobotModel(new robotModel::Ev3GeneratorRobotModel(kitId(), "ev3UsbGeneratorRobot", usbRobotName
			, usbRobotFriendlyName, usbPriority, new communication::UsbRobotCommunicationThread))
	, mBluetoothRobotModel(new robotModel::Ev3GeneratorRobotModel(kitId(), "ev3BluetoothGeneratorRobot"
			, bluetoothRobotName, bluetoothRobotFriendlyName
			, bluetoothPriority, new communication::BluetoothRobotCommunicationThread))
	, mBlocksFactory(new blocks::Ev3BlocksFactory({}))
{
}

Ev3GeneratorPluginBase::~Ev3GeneratorPluginBase()
{
}

QString Ev3GeneratorPluginBase::kitId() const
{
	return "ev3Kit";
}

QList<kitBase::robotModel::RobotModelInterface *> Ev3GeneratorPluginBase::robotModels()
{
	return { mUsbRobotModel.data(), mBluetoothRobotModel.data() };
}

kitBase::blocksBase::BlocksFactoryInterface *Ev3GeneratorPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	if (robotModels().contains(const_cast<kitBase::robotModel::RobotModelInterface *>(model))) {
		return mBlocksFactory;
	} else {
		return nullptr;
	}
}

QList<kitBase::AdditionalPreferences *> Ev3GeneratorPluginBase::settingsWidgets()
{
	return {};
}

void Ev3GeneratorPluginBase::onCurrentRobotModelChanged(kitBase::robotModel::RobotModelInterface &model)
{
	for (const qReal::ActionInfo &action : customActions()) {
		if (action.isAction()) {
			action.action()->setVisible(mUsbRobotModel.data() == &model || mBluetoothRobotModel.data() == &model);
		} else {
			action.menu()->setVisible(mUsbRobotModel.data() == &model || mBluetoothRobotModel.data() == &model);
		}
	}
}

void Ev3GeneratorPluginBase::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo)
}

communication::Ev3RobotCommunicationThread *Ev3GeneratorPluginBase::currentCommunicator()
{
	if (auto robotModel = dynamic_cast<robotModel::Ev3GeneratorRobotModel *>(&mRobotModelManager->model())) {
		return robotModel->communicator();
	}

	return nullptr;
}
