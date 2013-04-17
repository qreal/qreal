#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtGui/QKeyEvent>

#include "hotKeyAction.h"

namespace qReal {

class HotKeyManager
{
public:
	static HotKeyManager* instance();

	/// Sets a new command. Connect (on signal triggered()) and key sequence are made by component.
	/// @param id Command id.
	/// @param label Short description of command
	/// @param cmd QAction object which contains shortcut and with whom connection is performed.
	static void setCommand(QString id, QString label, HotKeyAction *cmd);

	/// Sets a new shortcut to existing command
	/// @param id Command id
	/// @param keyseq New shortcut
	static bool setShortcut(QString id, QKeySequence keyseq);

	static void doShortCut(QKeyEvent *keyevent);

private:
	HotKeyManager();

	void setCmd(QString id, HotKeyAction *cmd);
	bool setScut(QString id, QKeySequence keyseq);
	void addNewShortcut(QString id, QString const shortcut);
	void findShortcut(const QString shortcut);

	static HotKeyManager* mInstance;
	QHash<QString, HotKeyAction *> mCmds;
	QHash<QString, QString> mShortCuts;
};

}
