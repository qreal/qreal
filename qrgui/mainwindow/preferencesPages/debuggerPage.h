#pragma once

#include "page.h"

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

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesDebuggerPage *ui;
};
