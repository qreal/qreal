#pragma once

#include <QtWidgets/QLineEdit>

#include <qrgui/preferencesDialog/preferencesPage.h>

namespace Ui {
	class visualInterpreterPreferencesPage;
}

namespace qReal {

/// Preferences page for visual interpreter. Seems to be more or less useless rigth now.
class VisualInterpreterPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit VisualInterpreterPreferencesPage(QWidget *parent = 0);
	~VisualInterpreterPreferencesPage();

	void save();
	void restoreSettings();
private slots:
	void setQRealSourcesLocation();
	void setPythonPath();
	void setTempScriptPath();

private:
	Ui::visualInterpreterPreferencesPage *mUi;
};

}

