/* Copyright 2007-2015 QReal Research Group
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

#include "databaseSchemeGenerator.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace utils;

DatabaseEditorSchemeGenerator::DatabaseEditorSchemeGenerator(const LogicalModelAssistInterface &api
		, ErrorReporterInterface &errorReporter)
	: mApi(api)
	, mErrorReporter(errorReporter)
{
	fillKeyWords();
}

void DatabaseEditorSchemeGenerator::fillKeyWords()
{
	QFile sqlKeyWords("../plugins/ains/generators/databaseSchemeGenerator/SQLKeyWords.txt");
	QString word;
	if (sqlKeyWords.open(QIODevice::ReadOnly)) {
		QTextStream keyWords(&sqlKeyWords);
		while (!keyWords.atEnd()) {
			word = keyWords.readLine();
			word.trimmed();
			if ((word != "") && (word != "\n")&& (word != "	" ))
				mKeyWords << word;
		}
		sqlKeyWords.close();
	}

}

bool DatabaseEditorSchemeGenerator::isKeyWord(const QString &name)
{
	return mKeyWords.contains(name.toUpper());
}

QHash<Id, QPair<QString, QString>> DatabaseEditorSchemeGenerator::modelList()
{
	Id repoId = Id::rootId();
	IdList models = mApi.children(repoId);
	QHash<Id, QPair<QString, QString>> currentModelList;

	foreach (Id modelId, models) {
		QString const elementType = modelId.element();
		if (elementType == "DatabaseDiagram" && mApi.isLogicalId(modelId)) {
			QString directoryName = mApi.logicalRepoApi().stringProperty(modelId, "saveToTheDirectory");
			QString fileName = mApi.logicalRepoApi().stringProperty(modelId, "filename");
			QPair<QString, QString> fileAttributes;
			if (directoryName.isEmpty() || fileName.isEmpty()) {
				mErrorReporter.addError(QObject::tr("no directory or filename"), modelId);
				return QHash<Id, QPair<QString, QString> >();
			}
			fileAttributes.first = directoryName;
			fileAttributes.second = fileName;
			currentModelList.insert(modelId,fileAttributes);
		}
	}

	return currentModelList;
}

ErrorReporterInterface& DatabaseEditorSchemeGenerator::generateDatabaseScheme(const Id &modelId
		, const QString &pathToFile)
{
	IdList tableList = mApi.children(modelId);
	bool fileOpened = false;
	OutFile sqlFile(pathToFile + ".sql", &fileOpened);
	foreach (Id tableId, tableList) {
		QString const tableType = tableId.element();
		if (tableType == "Table" && mApi.isLogicalId(tableId)) {
			QString tableName = mApi.logicalRepoApi().stringProperty(tableId, "name");
			if (isKeyWord(tableName))
				mErrorReporter.addError(QObject::tr("using reserved key as name of table"), tableId);
			sqlFile() << QString("CREATE TABLE %1\n").arg(tableName);
			processingColumns(tableId, sqlFile);
			sqlFile() <<"\n\n";
		}
	}

	if (!fileOpened) {
		mErrorReporter.addCritical(QObject::tr("incorrect file path"));
	}

	return mErrorReporter;
}

void DatabaseEditorSchemeGenerator::processingColumns(const Id &tableId, utils::OutFile &outFile)
{
	IdList columns = mApi.children(tableId);
	outFile() << "(\n";
	bool start = true;
	foreach (Id id, columns) {
		if (!start) {
			outFile() << ",\n";
		}
		start = false;
		QString name = mApi.logicalRepoApi().stringProperty(id, "name");
		if (name == "") {
			mErrorReporter.addError(QObject::tr("no name of the element"), id);
			return;
		}
		QString isPrimaryKey = mApi.logicalRepoApi().stringProperty(id, "is Primary Key");
		QString type = columnType(id);
		QString primaryKey = ((isPrimaryKey == "true") ? "primary key" : "");
		outFile() << QString("%1 %2 %3").arg(name, type, primaryKey);
	}

	addForeignKeys(tableId, outFile);
	outFile() << "\n";
	outFile() << ");";
}

void DatabaseEditorSchemeGenerator::addForeignKeys(const Id &tableId, utils::OutFile &outFile)
{
	IdList foreignKeys = mApi.logicalRepoApi().outgoingLinks(tableId);

	foreach (Id id, foreignKeys) {
		outFile() << ",\n";
		if (id.element() == "Connection") {
			QString foreignKeyName = mApi.logicalRepoApi().stringProperty(id, "Foreign Key");
			if (foreignKeyName == "") {
				mErrorReporter.addError(QObject::tr("no name of foreign key"), id);
				return;
			}
			Id const connectedId = mApi.logicalRepoApi().to(id);
			QString const type = connectedId.element();
			if (isColumn(type)) {
				Id columnParentId = mApi.logicalRepoApi().parent(connectedId);
				if (columnParentId.element() == "Table")
					outFile() << QString("%1 %4 references %2(%3)").arg(
							foreignKeyName
							, mApi.logicalRepoApi().name(columnParentId)
							, mApi.logicalRepoApi().name(connectedId)
							, columnType(connectedId)
							);
			}
		}
	}
}

bool DatabaseEditorSchemeGenerator::isColumn(const QString &type)
{
	return ((type == "ColumnString") || (type == "ColumnNumber") || (type == "ColumnDate") || (type == "ColumnOther"));
}

const QString DatabaseEditorSchemeGenerator::columnType(const Id &id)
{
	QString const columnType = id.element();
	QString specificColumnType;
	if (mApi.isLogicalId(id)) {
		if (columnType == "ColumnString") {
			QString type = mApi.logicalRepoApi().stringProperty(id, "StringTypeProperty");
			QString maxtypeLength = mApi.logicalRepoApi().stringProperty(id, "maxtypeLength");
			if (maxtypeLength == "")
				specificColumnType = type;
			else
				specificColumnType = QString("%1(%2)").arg(type, maxtypeLength);
		}
		else if (columnType == "ColumnNumber") {
			QString type = mApi.logicalRepoApi().stringProperty(id, "NumTypeProperty");
			QString numberOfSigns = mApi.logicalRepoApi().stringProperty(id, "numberOfSigns");
			QString numberOfSignAfterPoint = mApi.logicalRepoApi().stringProperty(id, "numberOfSignAfterPoint");
			specificColumnType = type;
			if ((type == "decimal") || (type == "numeric")) {
				if (numberOfSigns.toInt() < numberOfSignAfterPoint.toInt())
					mErrorReporter.addError(QObject::tr("wrong parametres of type 'decimal' or 'numeric'"), id);
				if (numberOfSignAfterPoint != "0")
					specificColumnType += QString("(%1, %2)").arg(numberOfSigns, numberOfSignAfterPoint);
				else if (numberOfSigns != "0")
					specificColumnType += QString("(%1)").arg(numberOfSigns);
			}
		}
		else if ((columnType == "ColumnDate") || (columnType == "ColumnOther")) {
			QString type = mApi.logicalRepoApi().stringProperty(id, "TypeProperty");
			specificColumnType = type;
		}
	}

	return specificColumnType;
}
