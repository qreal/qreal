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

	/// Sets a new command. Connect (on signal triggered()) and key sequences are made by component.
	/// @param id Command id.
	/// @param label Short description of command
	/// @param command QAction object which contains shortcuts and with whom connection is performed.
	static void setCommand(QString const &id, QString const &label, QAction *command);

	static void deleteCommand(QString const &id);

	/// Sets a new shortcut to existing command
	/// @param id Command id
	/// @param keyseq Shortcut
	static bool setShortcut(QString const &id, QKeySequence const &keyseq);
	static bool setShortcut(QString const &id, QKeySequence const &modifier, MouseButtons mousebutton);

	static void resetShortcuts(QString const &id);
	static void resetAllShortcuts();
	static void deleteShortcut(QString const &id, QString const &shortcut);

	static void doShortcut(QEvent *event);

	static void setCurrentModifier(QString const &modifier);

	static QHash<QString, QAction *> commands();
	static QHash<QString, QString> shortcuts();

	static QString sequence(QString const &modifier, MouseButtons mousebutton);

private:
	HotKeyManager();
	HotKeyManager(HotKeyManager const&);

	void operator=(HotKeyManager const&);

	void registerCommand(QString const &id, QAction *command);

	bool registerShortcut(QString const &id, QKeySequence const &keyseq);
	bool registerShortcut(QString const &id, QKeySequence const &modifier, MouseButtons mousebutton);
	void registerShortcut(QString const &id, QString const &shortcut);

	void findShortcut(QString const &shortcut);

	void setCurrentModifierPrivate(QString const &modifier);
	QString currentModifier();

	void resetShortcutsPrivate(QString const &id);
	void resetAllShortcutsPrivate();
	void deleteShortcutPrivate(QString const &id, QString const &shortcut);

	void deleteCommandPrivate(QString const &id);
	bool hasPrefixOf(QString const &keyseq);
	void addPrefixes(QString const &keyseq);
	void deletePrefixes(QString const &keyseq);

	QHash<QString, QAction *> commandsPrivate();
	QHash<QString, QString> shortcutsPrivate();

	QString mCurrentModifer;
	QHash<QString, QAction *> mCommands;
	QHash<QString, QString> mShortcuts;
	QHash<QString, int> mPrefixes;
};

}
