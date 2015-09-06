#include "databasesReverseEngineer.h"

namespace qReal {
namespace databasesSupport {

DatabasesReverseEngineer::DatabasesReverseEngineer(PluginConfigurator const configurator
		, const DatabasesPreferencesPage *preferencesPage)
		: mErrorReporter(configurator.mainWindowInterpretersInterface().errorReporter())
{

}

void DatabasesReverseEngineer::generateSchema()
{
	QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
	sdb.setDatabaseName(qApp->applicationDirPath()
						+ QDir::separator()
						+ "ex" );

	if (!sdb.open()) {
		mErrorReporter->addError(QString("File didn`t open"));
	}
	else {
		mErrorReporter->addInformation("File opened successfully");
	}
}

}
}
