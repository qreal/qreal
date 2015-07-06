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

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtGui/QKeyEvent>

#include "qrgui/hotKeyManager/hotKeyManagerDeclSpec.h"

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

class QRGUI_HOTKEY_MANAGER_EXPORT HotKeyManager
{
public:
	static HotKeyManager& instance();

	/// Sets a new command. Connect (on signal triggered()) and key sequences are made by component.
	/// @param id Command id.
	/// @param label Short description of command
	/// @param command QAction object which contains shortcuts and with whom connection is performed.
	static void setCommand(const QString &id, const QString &label, QAction *command);

	static void deleteCommand(const QString &id);

	/// Sets a new shortcut to existing command
	/// @param id Command id
	/// @param keyseq Shortcut
	static bool setShortcut(const QString &id, const QKeySequence &keyseq);

	static void resetShortcuts(const QString &id);
	static void resetAllShortcuts();
	static void deleteShortcut(const QString &id, const QString &shortcut);

	static QHash<QString, QAction *> commands();
	static QHash<QString, QString> shortcuts();

private:
	HotKeyManager();
	explicit HotKeyManager(HotKeyManager const&);
	~HotKeyManager();

	void operator=(HotKeyManager const&);

	void registerCommand(const QString &id, QAction *command);

	bool registerShortcut(const QString &id, const QKeySequence &keyseq);
	void registerShortcut(const QString &id, const QString &shortcut);

	void findShortcut(const QString &shortcut);

	void resetShortcutsPrivate(const QString &id);
	void resetAllShortcutsPrivate();
	void deleteShortcutPrivate(const QString &id, const QString &shortcut);

	void deleteCommandPrivate(const QString &id);
	bool hasPrefixOf(const QString &keyseq);
	void addPrefixes(const QString &keyseq);
	void deletePrefixes(const QString &keyseq);

	QHash<QString, QAction *> commandsPrivate();
	QHash<QString, QString> shortcutsPrivate();

	QHash<QString, QAction *> mCommands;
	QHash<QString, QString> mShortcuts;
	QHash<QString, int> mPrefixes;
};

}
