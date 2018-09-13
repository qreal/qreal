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

#pragma once

#include <kitBase/additionalPreferences.h>

namespace Ui {
class PioneerAdditionalPreferences;
}

namespace pioneer {
namespace lua {

/// Widget for additional preferences for Pioneer quadcopter. Allows to set IP addres of Pioneer base station.
class PioneerAdditionalPreferences : public kitBase::AdditionalPreferences
{
	Q_OBJECT

public:
	explicit PioneerAdditionalPreferences(QWidget *parent = nullptr);
	~PioneerAdditionalPreferences() override;

	void save() override;
	void restoreSettings() override;
	void onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel) override;

signals:
	/// Emitted when some settings are changed.
	void settingsChanged();

private:
	Ui::PioneerAdditionalPreferences *mUi;
};

}
}
