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
	void doubleClicked(int const row, int const column);
	void activateShortcutLineEdit(int const row, int const column);
	void newModifiers(Qt::KeyboardModifiers modifiers);
	void newKey(int const key);
	void resetShortcuts();
	void resetAllShortcuts();

protected:
	void showEvent(QShowEvent *e);

private:
	void loadHotKeys();
	void initTable();
	void setTextColor(QColor const &color);

	Ui::hotKeyManagerPage *mUi;

	QString mCurrentId;
	QTableWidgetItem *mCurrentItem;
	Qt::KeyboardModifiers mCurrentModifiers;
};
