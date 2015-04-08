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

#include <QtGui/QKeySequence>
#include <QtWidgets/QTableWidgetItem>

#include <qrgui/preferencesDialog/preferencesPage.h>

#include "qrgui/hotKeyManager/hotKeyManagerDeclSpec.h"

namespace Ui {
	class hotKeyManagerPage;
}

class QRGUI_HOTKEY_MANAGER_EXPORT PreferencesHotKeyManagerPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesHotKeyManagerPage(QWidget *parent = 0);
	~PreferencesHotKeyManagerPage();

	void save();
	void restoreSettings();

private slots:
	void doubleClicked(const int row, const int column);
	void activateShortcutLineEdit(const int row, const int column);
	void newModifiers(Qt::KeyboardModifiers modifiers);
	void newKey(const int key);
	void resetShortcuts();
	void resetAllShortcuts();

protected:
	void showEvent(QShowEvent *e);

private:
	void loadHotKeys();
	void initTable();
	void setTextColor(const QColor &color);

	Ui::hotKeyManagerPage *mUi;

	QString mCurrentId;
	QTableWidgetItem *mCurrentItem;
	Qt::KeyboardModifiers mCurrentModifiers;
};
