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

#include "interpreterCore/managers/robotModelManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;
using namespace kitBase::robotModel;

RobotModelManager::RobotModelManager()
	: mRobotModel(&mDefaultRobotModel)
{
}

RobotModelInterface &RobotModelManager::model() const
{
	return *mRobotModel;
}

void RobotModelManager::setModel(RobotModelInterface * const robotModel)
{
	if (mRobotModel != robotModel) {
		disconnect(mRobotModel);
		const auto actualModel = robotModel ? robotModel : &mDefaultRobotModel;
		/// @todo implement hierarchical structure in settings manager
		/// @todo if some settings key is modified here do not forget to modify
		/// it in UsbRobotCommunicationThread::checkConsistency
		const QString selectedKit = actualModel->kitId();
		qReal::SettingsManager::setValue("SelectedRobotKit", selectedKit);
		/// @todo select kit here if needed
		const QString key = "SelectedModelFor" + selectedKit;
		qReal::SettingsManager::setValue(key, actualModel->name());
		mRobotModel = actualModel;

		connect(mRobotModel, &RobotModelInterface::connected, this, &RobotModelManager::connected);
		connect(mRobotModel, &RobotModelInterface::disconnected, this, &RobotModelManager::disconnected);
		connect(mRobotModel, &RobotModelInterface::allDevicesConfigured
				, this, &RobotModelManager::allDevicesConfigured);

		mRobotModel->init();

		emit robotModelChanged(*mRobotModel);
	}
}

void RobotModelManager::sync()
{
	emit robotModelChanged(*mRobotModel);
}
