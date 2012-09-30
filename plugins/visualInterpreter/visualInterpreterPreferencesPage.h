#pragma once

#include "../../qrgui/dialogs/preferencesPages/preferencesPage.h"

#include <QtGui/QLineEdit>

namespace Ui {
	class visualInterpreterPreferencesPage;
}

namespace qReal {

/// Preferences page for visual interpreter. Seems to be more or less useless rigth now.
class VisualInterpreterPreferencesPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit VisualInterpreterPreferencesPage(QWidget *parent = 0);
	~VisualInterpreterPreferencesPage();

	void save();

private slots:
	void setQRealSourcesLocation();

private:
	Ui::visualInterpreterPreferencesPage *mUi;
};

}

