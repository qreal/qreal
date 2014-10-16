#include "databasesPreferencesPage.h"
#include <QtWidgets/QFileDialog>
//#include "ui_viewInteractionPreferencesPage.h"

#include "../../../qrkernel/settingsManager.h"

namespace qReal {
namespace databasesSupport {

DatabasesPreferencesPage::DatabasesPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::DatabasesPreferencesPage())
{
	mUi->setupUi(this);
}

DatabasesPreferencesPage::~DatabasesPreferencesPage()
{
	delete mUi;
}

void DatabasesPreferencesPage::save()
{
}

void DatabasesPreferencesPage::restoreSettings()
{
}

}
}
