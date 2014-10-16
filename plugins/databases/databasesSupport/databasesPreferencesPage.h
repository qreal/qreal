#pragma once

#include <QtCore/QTranslator>
#include <QtWidgets/QLineEdit>

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"
#include "ui_databasesPreferencesPage.h"

namespace Ui {
	class DatabasesPreferencesPage;
}

namespace qReal {
namespace databasesSupport {

class DatabasesPreferencesPage : public PreferencesPage
{
public:
	explicit DatabasesPreferencesPage(QWidget *parent = 0);
	~DatabasesPreferencesPage();

	void save();
	virtual void restoreSettings();
private:
	Ui::DatabasesPreferencesPage *mUi;
};

}
}
