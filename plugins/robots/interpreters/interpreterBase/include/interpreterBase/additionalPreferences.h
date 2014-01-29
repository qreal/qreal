#pragma once

#include <qrgui/dialogs/preferencesPages/preferencesPage.h>
#include "interpreterBaseDeclSpec.h"
#include "robotModel/robotModelInterface.h"

namespace interpreterBase {

/// A base class for additional widget on the robots settings page. Can be subclassed in concrete plugins.
class ROBOTS_INTERPRETER_BASE_EXPORT AdditionalPreferences : public PreferencesPage
{
public:
	explicit AdditionalPreferences(QWidget *parent = 0)
		: PreferencesPage(parent)
	{
	}

	virtual ~AdditionalPreferences()
	{
	}

	/// Called each time when user selects another robot model
	virtual void onRobotModelChanged(robotModel::RobotModelInterface * const robotModel)
	{
		Q_UNUSED(robotModel)
	}
};

}
