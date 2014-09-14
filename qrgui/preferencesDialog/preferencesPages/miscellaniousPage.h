#pragma once

#include "preferencesDialog/preferencesPage.h"

namespace Ui {
	class PreferencesMiscellaniousPage;
}

class PreferencesMiscellaniousPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesMiscellaniousPage(QWidget *parent = 0);
	~PreferencesMiscellaniousPage();

	void save();
	virtual void restoreSettings();

signals:
	void iconsetChanged();
	void toolbarSizeChanged(int size);

protected:
	void changeEvent(QEvent *e);

private slots:
	void browseImagesPath();

private:
	Ui::PreferencesMiscellaniousPage *mUi;

	QString mLastIconsetPath;
};
