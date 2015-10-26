#pragma once

#include <QObject>
#include <QMessageBox>
#include <QtSql>

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
	void generateSchema(QString const &dbms, QString const &filePath);
private:
	/// Creates element of diagram from string
	qReal::Id createElementFromString(QString const &elemName
			, QPointF coord = QPointF(0,0)
			, Id const &parentLogicalId = Id::rootId()
			, bool coordByParent = false); // true means, that coordinates will be taken from parent
	qReal::Id createTable(QString const &tableName
			, QPointF const &coord = QPointF(0,0)
			, Id const &parentLogicalId = Id::rootId());
	qReal::Id createColumn(QString const &columnName
			, QString const &columnType
			, Id const &parentId);

	QString mDbms;

	// do not make const & (some methods don`t work)
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	qReal::ErrorReporterInterface *mErrorReporter;
	DatabasesPreferencesPage const *mPreferencesPage;
};

}
}
