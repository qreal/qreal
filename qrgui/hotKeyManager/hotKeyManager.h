#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtGui/QKeyEvent>

namespace qReal {

enum MouseButtons
{
	MouseLB
	, MouseRB
	, MouseMB
	, MouseWU
	, MouseWD
};

class HotKeyManager
{
public:
	static HotKeyManager* instance();

	/// Sets a new command. Connect (on signal triggered()) and key sequence are made by component.
	/// @param id Command id.
	/// @param label Short description of command
	/// @param cmd QAction object which contains shortcut and with whom connection is performed.
	static void setCommand(QString id, QString label, QAction *cmd);

	static void delCommand(QString id);

	/// Sets a new shortcut to existing command
	/// @param id Command id
	/// @param keyseq Shortcut
	static bool setShortcut(QString id, QKeySequence keyseq);
	static bool setShortcut(QString id, Qt::Modifier mod, MouseButtons mb);

	static void resetCmdShortcuts(QString id);
	static void resetAllCmdsShortcuts();

	static void doShortcut(QEvent *event);

	static void setCurrentModifier(QString mod);

private:
	HotKeyManager();

	void setCmd(QString id, QAction *cmd);

	bool setScut(QString id, QKeySequence keyseq);
	bool setScut(QString id, Qt::Modifier mod, MouseButtons mb);

	void addNewShortcut(QString id, QString const shortcut);

	void findShortcut(const QString shortcut);

	QString sequence(QString mod, MouseButtons mb);

	void setCurrMod(QString mod);
	QString currentModifier();

	void resetShortcuts(QString id);
	void resetAllShortcuts();

	void delCmd(QString id);

	static HotKeyManager* mInstance;
	QString mCurrentModifer;
	QHash<QString, QAction *> mCmds;
	QHash<QString, QString> mShortCuts;
};

}
