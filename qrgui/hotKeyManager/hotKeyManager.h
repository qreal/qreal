#pragma once

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QList>

#include "hotKeyAction.h"
#include "hotKeyManagerTypes.h"

namespace qReal {

class HotKeyManager
{
public:
	static HotKeyManager* instance();

	/// Sets a new command. Connect (on signal pressed()) and key sequence are made by component.
	/// @param id Command id.
	/// @param label Short description of command
	/// @param cmd QAction object which contains shortcut and with whom connection is performed.
	static void setCommand(QString const id, QString const label, HotKeyAction *cmd);

	/// Sets a new shortcut to existing command
	/// @param id Command id
	/// @param keyseq New shortcut
	static bool addShortcut(QString const id, QKeySequence keyseq);
	static bool addShortcut(QString const id, QKeySequence keyseq, MouseShortcuts mouseShortcut);

	static void doShortcut(QKeyEvent *keyevent);
	static void doShortcut(QWheelEvent *wheelevent);
	static void doShortcut(QMouseEvent *mouseevent);

	static void clearCurrentKeySeq();

	static void resetCmdShortcuts(QString const id);

private:
	HotKeyManager();

	void setCmd(QString const id, HotKeyAction *cmd);
	bool addNewShortcut(QString const id, QString const shortcut);
	bool addShortcut(QString const id, QString const shortcut);
	void findShortcut(QString const shortcut);
	void setCurrentKeySeq(QKeySequence keyseq);
	QString sequence(QString const keyseq, MouseShortcuts msc);
	QKeySequence currentKeySeq();
	void resetShortcuts(QString const id);

	static HotKeyManager* mInstance;
	QKeySequence mCurrentKeySeq;
	QHash<QString, HotKeyAction *> mCmds;
	QHash<QString, QString> mShortCuts;
	//QHash<QString, QHash<MouseShortcuts, QString> > mMouseShortcuts;
};

}
