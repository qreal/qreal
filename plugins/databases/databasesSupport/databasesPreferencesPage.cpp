#include "databasesPreferencesPage.h"
#include <QtWidgets/QFileDialog>
//#include "ui_viewInteractionPreferencesPage.h"

#include "../../../qrkernel/settingsManager.h"

namespace qReal {
namespace databasesSupport {

DatabasesPreferencesPage::DatabasesPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
{
}

DatabasesPreferencesPage::~DatabasesPreferencesPage()
{
	//delete mUi;
}

void DatabasesPreferencesPage::save()
{
}

void DatabasesPreferencesPage::restoreSettings()
{
}

}
}
