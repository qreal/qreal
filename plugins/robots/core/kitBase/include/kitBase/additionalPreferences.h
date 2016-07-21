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

#include <qrgui/preferencesDialog/preferencesPage.h>

#include "kitBase/robotModel/robotModelInterface.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {

/// A base class for additional widget on the robots settings page. Can be subclassed in concrete plugins.
class ROBOTS_KIT_BASE_EXPORT AdditionalPreferences : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parent - parent widget in terms of Qt widget system.
	explicit AdditionalPreferences(QWidget *parent = 0)
		: PreferencesPage(parent)
	{
	}

	/// Called each time when user selects another robot model.
	/// @param robotModel - newly selected robot model.
	virtual void onRobotModelChanged(robotModel::RobotModelInterface * const robotModel)
	{
		Q_UNUSED(robotModel)
	}
};

}
