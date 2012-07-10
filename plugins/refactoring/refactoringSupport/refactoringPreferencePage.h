#pragma once

#include <QtGui/QLineEdit>

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class refactoringPreferencesPage;
}

namespace qReal {

/// Preferences page for refactoring manager. Seems to be more or less useless rigth now.
class RefactoringPreferencesPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit RefactoringPreferencesPage(QWidget *parent = 0);
	~RefactoringPreferencesPage();

	void save();

protected:
	void changeEvent(QEvent *e);

private slots:
	void setQRealSourcesLocation();

private:
	Ui::refactoringPreferencesPage *mUi;
};

}

