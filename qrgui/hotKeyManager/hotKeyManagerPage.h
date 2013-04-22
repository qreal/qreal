#pragma once

#include "../dialogs/preferencesPages/preferencesPage.h"

#include <QtGui/QKeySequence>

namespace Ui {
	class hotKeyManagerPage;
}

class PreferencesHotKeyManagerPage : public PreferencesPage
{
	Q_OBJECT
	
public:
	explicit  PreferencesHotKeyManagerPage(QWidget *parent = 0);
	~ PreferencesHotKeyManagerPage();

	void save();
	void restoreSettings();

	void loadHotKeys();

private slots:
	void activateShortcutLineEdit(int row, int column);
	void newShortcut(QKeySequence keyseq);

private:
	int mCurrentRow;

	Ui::hotKeyManagerPage *mUi;
};
