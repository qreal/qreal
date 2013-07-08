#pragma once

#include "preferencesPage.h"

namespace Ui {
	class PreferencesMiscellaniousPage;
}

class PreferencesMiscellaniousPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesMiscellaniousPage(QWidget *parent = 0);
	~PreferencesMiscellaniousPage();

	void save();
	virtual void restoreSettings();

signals:
	void iconsetChanged();

protected:
	void changeEvent(QEvent *e);

private slots:
	void browseImagesPath();

private:
	enum LineType {
		BrokenLine = 0, SquareLine, CurveLine
	};

	Ui::PreferencesMiscellaniousPage *mUi;

	QString mLastIconsetPath;
};
