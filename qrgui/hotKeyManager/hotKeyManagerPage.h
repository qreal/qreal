#pragma once

#include "../dialogs/preferencesPages/preferencesPage.h"

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
	
private:
	Ui::hotKeyManagerPage *mUi;
};
