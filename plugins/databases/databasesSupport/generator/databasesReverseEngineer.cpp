#include "databasesReverseEngineer.h"

namespace qReal {
namespace databasesSupport {

DatabasesReverseEngineer::DatabasesReverseEngineer(PluginConfigurator const configurator
		, const DatabasesPreferencesPage *preferencesPage)
		: mErrorReporter(configurator.mainWindowInterpretersInterface().errorReporter())
		, mLogicalModelApi(configurator.logicalModelApi())
		, mGraphicalModelApi(configurator.graphicalModelApi())
		, mInterpretersInterface(configurator.mainWindowInterpretersInterface())
		, mPreferencesPage(preferencesPage)
		, mDbms(QString("microsoftAccess"))
{

}

qReal::Id DatabasesReverseEngineer::createElementFromString(QString const &elemName
		, QPointF coord
		, Id const &parentLogicalId
		, bool coordByParent)
{
	Id id = Id::loadFromString(QString("qrm:/" + mDbms
			+ "/DatabasesPhysicalModelMetamodel/" + elemName));
	Id logicalId = mLogicalModelApi.createElement(Id::rootId(), id);
	Id graphicalParentId = Id::rootId();
	if (parentLogicalId != Id::rootId()) {
		graphicalParentId = mGraphicalModelApi.graphicalIdsByLogicalId(parentLogicalId).first();
		if (coordByParent)
			coord = mGraphicalModelApi.position(graphicalParentId);
	}
	mGraphicalModelApi.createElement(graphicalParentId, logicalId, true, elemName, coord);
	return logicalId;
}

qReal::Id DatabasesReverseEngineer::createTable(QString const &tableName
		, QPointF const &coord, Id const &parentLogicalId)
{
	Id logicalTableId = createElementFromString("Table", coord, parentLogicalId);
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "Name");
	return logicalTableId;
}

qReal::Id DatabasesReverseEngineer::createColumn(QString const &columnName
		, QString const &columnType, Id const &parentId)
{
	Id logicalColumnId = createElementFromString("Column", QPointF(), parentId, true);
	mLogicalModelApi.setPropertyByRoleName(logicalColumnId, columnName, "Name");
	return logicalColumnId;
}

void DatabasesReverseEngineer::generateSchema()
{
	QString dbmsType = "";
	QString driverInitializerStr = "";
	if (mDbms == "Microsoft Access")
	{
		dbmsType = "QODBC";
		driverInitializerStr = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MSAccess};DBQ=";
	}

	QSqlDatabase sdb = QSqlDatabase::addDatabase(dbmsType);
	QString filename = mPreferencesPage->getReverseEngineerFilename();
	sdb.setDatabaseName(QString(driverInitializerStr + filename));

	if (!sdb.open()) {
		mErrorReporter->addError(QString(tr("File didn`t open")));
		return;
	}
	else {
		mErrorReporter->addInformation(QString(tr("File opened successfully")));
	}

	QString curEditorName = mDbms;
	mDbms = QString(mDbms.at(0).toUpper()) + curEditorName.remove(0,1);
	mErrorReporter->clear();

	Id logicalDiagramId = createElementFromString("DatabasesPhysicalNode");

	QStringList tableNames = sdb.tables();
	QPointF coord = QPointF(10,10);
	for (QString const &tableName : tableNames)
	{
		Id logicalTableId = createTable(tableName, coord, logicalDiagramId);
		coord = QPointF(coord.rx() + 50, coord.ry() + 50);

		QSqlRecord columns = sdb.record(tableName);
		int columnsCount = columns.count();
		for (int i = 0; i < columnsCount ; i++)
		{
			QSqlField const &column = columns.field(i);

			QVariant type = column.type();
			Id logicalColumnId = createColumn(column.name(), type.toString(), logicalTableId);
		}

	QStringList connections = sdb.connectionNames();

	mDbms = curEditorName;

}
}


}
}
