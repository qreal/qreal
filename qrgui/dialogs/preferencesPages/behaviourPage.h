#pragma once

#include "dialogs/preferencesPages/preferencesPage.h"
#include "mainwindow/filterObject.h"

namespace Ui {
	class PreferencesBehaviourPage;
}

class PreferencesBehaviourPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesBehaviourPage(QWidget *parent = 0);
	~PreferencesBehaviourPage();

	void save();
	virtual void restoreSettings();

signals:
	void usabilityTestingModeChanged(bool on);

protected:
	void changeEvent(QEvent *e);

private slots:
	void showAutoSaveBox(bool show);

private:
	Ui::PreferencesBehaviourPage *mUi;
	FilterObject mFilterObject;
	bool mUsabilityTestingMode;
};
