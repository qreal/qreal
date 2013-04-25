#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtGui/QKeyEvent>

namespace qReal {

enum MouseButtons
{
	None
	, MouseLB
	, MouseRB
	, MouseMB
	, MouseWU
	, MouseWD
};

class HotKeyManager
{
public:
	static HotKeyManager& instance();

	/// Sets a new command. Connect (on signal triggered()) and key sequence are made by component.
	/// @param id Command id.
	/// @param label Short description of command
	/// @param cmd QAction object which contains shortcut and with whom connection is performed.
	static void setCommand(QString const &id, QString const &label, QAction *cmd);

	static void deleteCommand(QString const &id);

	/// Sets a new shortcut to existing command
	/// @param id Command id
	/// @param keyseq Shortcut
	static bool setShortcut(QString const &id, QKeySequence const &keyseq);
	static bool setShortcut(QString const &id, QKeySequence const &mod, MouseButtons mb);

	static void resetShortcuts(const QString &id);
	static void resetAllShortcuts();

	static void doShortcut(QEvent *event);

	static void setCurrentModifier(QString const &mod);

	static QHash<QString, QAction *> commands();
	static QHash<QString, QString> cmdsShortcuts();

	static QString sequence(QString const &mod, MouseButtons mb);

private:
	HotKeyManager();
	HotKeyManager(HotKeyManager const&);

	void operator=(HotKeyManager const&);

	void registerCommand(QString const &id, QAction *cmd);

	bool registerShortcut(QString const &id, QKeySequence const &keyseq);
	bool registerShortcut(QString const &id, QKeySequence const &mod, MouseButtons mb);
	void registerShortcut(QString const &id, QString const &shortcut);

	void findShortcut(QString const &shortcut);

	void setCurrentModifierPrivate(QString const &mod);
	QString currentModifier();

	void resetShortcutsPrivate(QString const &id);
	void resetAllShortcutsPrivate();

	void deleteCommandPrivate(QString const &id);
	bool isPrefix(QString const &keyseq);

	QHash<QString, QAction *> cmds();
	QHash<QString, QString> shortcuts();

	QString mCurrentModifer;
	QHash<QString, QAction *> mCmds;
	QHash<QString, QString> mShortcuts;
};

}
