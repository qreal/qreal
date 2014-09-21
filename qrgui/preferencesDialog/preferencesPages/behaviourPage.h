#pragma once

#include "preferencesDialog/preferencesPage.h"
#include "mainWindow/filterObject.h"

namespace Ui {
	class PreferencesBehaviourPage;
}

class PreferencesBehaviourPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesBehaviourPage(QWidget *parent = 0);
	~PreferencesBehaviourPage() override;

	void save() override;
	void restoreSettings() override;

signals:
	void usabilityTestingModeChanged(bool on);

protected:
	void changeEvent(QEvent *e);

private slots:
	void showAutoSaveBox(bool show);

private:
	void initLanguages();

	Ui::PreferencesBehaviourPage *mUi;
	bool mUsabilityTestingMode;
	QString mOldLanguage;
};
