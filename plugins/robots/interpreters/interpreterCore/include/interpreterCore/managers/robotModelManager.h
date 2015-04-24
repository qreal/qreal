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

#include <kitBase/robotModel/robotModelInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

#include "interpreterCore/defaultRobotModel.h"

namespace interpreterCore {

/// Contains a reference to currently selected robot model and provides a notification when it changes.
/// This is the only place that has a valid up-to-date information about robot model, all other components shall
/// use it as a reference.
class RobotModelManager : public kitBase::robotModel::RobotModelManagerInterface
{
	Q_OBJECT

public:
	RobotModelManager();

	kitBase::robotModel::RobotModelInterface &model() const override;

public slots:
	/// Changes currently selected robot model and notifies all about change by robotModelChanged signal.
	void setModel(kitBase::robotModel::RobotModelInterface * const robotModel);

	/// Forces manager to send robotModelChanged signal with current model as an argument.
	void sync();

private:
	kitBase::robotModel::RobotModelInterface *mRobotModel;  // Does not have ownership.
	DefaultRobotModel mDefaultRobotModel;
};

}
