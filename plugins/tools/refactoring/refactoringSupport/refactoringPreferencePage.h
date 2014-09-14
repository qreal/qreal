#pragma once

#include <QtWidgets/QLineEdit>

#include <qrgui/preferencesDialog/preferencesPage.h>

namespace Ui {
	class refactoringPreferencesPage;
}

namespace qReal {

/// Preferences page for refactoring manager. Seems to be more or less useless rigth now.
class RefactoringPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit RefactoringPreferencesPage(QWidget *parent = 0);
	~RefactoringPreferencesPage();

	void save();
	void restoreSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void setQRealSourcesLocation();
	void changeDefaultDotPath();
	void setDotPathManually();

private:
	Ui::refactoringPreferencesPage *mUi;
};

}

