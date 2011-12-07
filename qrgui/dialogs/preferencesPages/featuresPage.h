#pragma once

#include "preferencesPage.h"

namespace Ui {
	class PreferencesFeaturesPage;
}

class PreferencesFeaturesPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesFeaturesPage(QWidget *parent = 0);
	~PreferencesFeaturesPage();

	void save();
	QIcon getIcon() const;

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesFeaturesPage *mUi;

	QIcon mIcon;
};
