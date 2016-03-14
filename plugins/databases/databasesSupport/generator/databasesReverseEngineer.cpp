/* Copyright 2014-2016 Anastasia Semenova
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
		, mDbms(QString("QMYSQL"))
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
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "tableName");
	return logicalTableId;
}

qReal::Id DatabasesReverseEngineer::createColumn(QString const &columnName
		, QString const &columnType, Id const &parentId)
{
	Id logicalColumnId = createElementFromString("Column", QPointF(), parentId, true);
	mLogicalModelApi.setPropertyByRoleName(logicalColumnId, columnName, "columnName");
	return logicalColumnId;
}

// TODO:
// QOCI Oracle Call Interface
// QPSQL  PostgreSQL
void DatabasesReverseEngineer::generateSchema(QString const &dbms, QString const &filePath)
{
	mDbms = dbms;

	QString dbmsType = "";
	QString driverInitializerStr = "";
	if (mDbms == "Sqlite") {
		dbmsType = "QSQLITE";
		driverInitializerStr = "";
	}
	else if (mDbms == "MicrosoftAccess") {
		dbmsType = "QODBC";
		driverInitializerStr = "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MSAccess};DBQ=";
	}

	QSqlDatabase sdb = QSqlDatabase::addDatabase(dbmsType);
	sdb.setDatabaseName(QString(driverInitializerStr + filePath));

	if (!sdb.open()) {
		mErrorReporter->addError(QString(tr("File didn`t open")));
		return;
	}
	else {
		mErrorReporter->addInformation(QString(tr("File opened successfully")));
	}

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

	}
	mErrorReporter->addInformation(QString("Schema generated succesfully"));
}


}
}
