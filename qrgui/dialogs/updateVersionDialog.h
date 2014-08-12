#pragma once

#include <QtWidgets/QWidget>

namespace qReal {

/// Tells user that new version is ready to be installed. Prompts user to make a decision:
/// update immediately, ask next time or never prompt this version any more.
/// If dialog accepted then user selected to install new version immediately.
/// Dialog may not be prompted if user asked to forget about this version.
class UpdateVersionDialog
{
public:
	static bool promptUpdate(QString const &newVersion, QWidget *parent = 0);

private:
	static QString readyMessage(QString const &version);
	static QString availableMessage(QString const &version);

	static bool isVersionDeclined(QString const &version);
	static void declineVersion(QString const &version);
	static QString declineSettingsKey(QString const &version);
};

}
