#pragma once

#include <qrgui/dialogs/preferencesPages/preferencesPage.h>

#include "robotModel/robotModelInterface.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {

/// A base class for additional widget on the robots settings page. Can be subclassed in concrete plugins.
class ROBOTS_INTERPRETER_BASE_EXPORT AdditionalPreferences : public PreferencesPage
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
