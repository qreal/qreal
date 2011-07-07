#pragma once

#include "page.h"

namespace Ui {
	class preferencesBehaviourPage;
}

class PreferencesBehaviourPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesBehaviourPage(QWidget *parent = 0);
	~PreferencesBehaviourPage();

	void save();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::preferencesBehaviourPage *ui;
};
