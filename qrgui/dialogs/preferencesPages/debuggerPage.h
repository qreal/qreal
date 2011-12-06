#pragma once

#include "preferencesPage.h"

namespace Ui {
	class PreferencesDebuggerPage;
}

class PreferencesDebuggerPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesDebuggerPage(QWidget *parent = 0);
	~PreferencesDebuggerPage();

	void save();
	QIcon getIcon();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesDebuggerPage *mUi;

	QIcon mIcon;
};
