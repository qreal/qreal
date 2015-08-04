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
	connect(mUi->dbmsBox, SIGNAL(currentTextChanged(QString)), this, SLOT(dbmsChanging(QString)));
}

DatabasesPreferencesPage::~DatabasesPreferencesPage()
{
	delete mUi;
}

void DatabasesPreferencesPage::dbmsChanging(QString const &dbmsName)
{
	if (dbmsName == "SQLite")
		emit dbmsChanged("sqlite");
	else if (dbmsName == "Microsoft SQL Server 2008")
		emit dbmsChanged("sqlServer2008");
	else if (dbmsName == "MySQL 5")
		emit dbmsChanged("mySql5");
}

void DatabasesPreferencesPage::save()
{
}

void DatabasesPreferencesPage::restoreSettings()
{
}

}
}
