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

	//void mousePressEvent(QMouseEvent *event);

private slots:
	void activateShortcutLineEdit(int const row, int const column);
	void newModifiers(QString const modifiers);
	void newKey(QString const key);

private:
	void loadHotKeys();
	void updateCurrentRow(QString const shortcut);

	Ui::hotKeyManagerPage *mUi;

	int mCurrentRow;
	QString mCurrentModifiers;
};
