#pragma once

#include "qrgui/preferencesDialog/preferencesPage.h"

namespace Ui {
	class PreferencesFeaturesPage;
}

class PreferencesFeaturesPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesFeaturesPage(QWidget *parent = 0);
	~PreferencesFeaturesPage();

	void save();
	virtual void restoreSettings();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesFeaturesPage *mUi;
};
