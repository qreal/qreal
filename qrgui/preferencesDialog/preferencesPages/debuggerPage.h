#pragma once

#include "preferencesDialog/preferencesPage.h"

namespace Ui {
	class PreferencesDebuggerPage;
}

class PreferencesDebuggerPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesDebuggerPage(QWidget *parent = 0);
	~PreferencesDebuggerPage();

	void save();
	virtual void restoreSettings();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesDebuggerPage *mUi;
};
