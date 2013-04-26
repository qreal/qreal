#pragma once

#include <QtGui/QKeySequence>
#include <QtWidgets/QTableWidgetItem>

#include "../dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class hotKeyManagerPage;
}

class PreferencesHotKeyManagerPage : public PreferencesPage
{
	Q_OBJECT
	
public:
	explicit  PreferencesHotKeyManagerPage(QWidget *parent = 0);
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

private:
	void loadHotKeys();

	Ui::hotKeyManagerPage *mUi;

	QString mCurrentId;
	QTableWidgetItem *mCurrentItem;
	Qt::KeyboardModifiers mCurrentModifiers;
};
