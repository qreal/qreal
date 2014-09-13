#pragma once

#include <QtWidgets/QWidget>

namespace updatesChecker {

/// Tells user that new version is ready to be installed. Prompts user to make a decision:
/// update immediately or to ask next time.
class UpdateVersionDialog
{
public:
	/// @return true if user wishes to update version now.
	static bool promptUpdate(QWidget *parent = 0);
};

}
