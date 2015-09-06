#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>

#include <plugins/toolPluginInterface/pluginConfigurator.h>

#include "databasesPreferencesPage.h"

namespace qReal {
namespace databasesSupport {

class DatabasesReverseEngineer : public QObject
{
	Q_OBJECT
public:
	DatabasesReverseEngineer(PluginConfigurator const configurator
		, DatabasesPreferencesPage const *preferencesPage);

signals:

public slots:
	void generateSchema();
private:
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}
