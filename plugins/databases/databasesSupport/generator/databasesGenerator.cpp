/* Copyright 2014-2015 Anastasia Semenova
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

#include "databasesGenerator.h"

namespace qReal {
namespace databasesSupport {

QString const databases = "Databases";

DatabasesGenerator::DatabasesGenerator(PluginConfigurator const configurator
		, DatabasesPreferencesPage const *preferencesPage)
	: mCurrentDiagram(Id::rootId())
	, mLogicalModelApi(configurator.logicalModelApi())
	, mGraphicalModelApi(configurator.graphicalModelApi())
	, mInterpretersInterface(configurator.mainWindowInterpretersInterface())
	, mErrorReporter(configurator.mainWindowInterpretersInterface().errorReporter())
	, mDbms(QString("SqlServer2008"))
	, mPreferencesPage(preferencesPage)
	, mMainWindowInterface(configurator.mainWindowInterpretersInterface())
	, mCodeGenerationMode(CreateTable)
{
	connect(mPreferencesPage, SIGNAL(dbmsChanged(QString)), this, SLOT(changeEditor(QString)));
}

DatabasesGenerator::~DatabasesGenerator()
{
}

void DatabasesGenerator::clearPhysicalModel()
{
	IdList tableNodes = findNodes("DatabasesPhysicalNode");
	for (Id const &id : tableNodes) {
		mLogicalModelApi.removeElement(id);
	}
}

IdList DatabasesGenerator::findNodes(QString const &name)
{
	IdList const children = mGraphicalModelApi.graphicalRepoApi().children(mCurrentDiagram);
	IdList filteredChildren;
	for (Id const &child : children) {
		if (child.element() == name) {
			filteredChildren.append(child);
		}
	}
	return filteredChildren;
}

QVariant DatabasesGenerator::getProperty(Id const &id, QString const &propertyName)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	}

	return mLogicalModelApi.logicalRepoApi().property(mGraphicalModelApi.logicalId(id), propertyName);
}

Id DatabasesGenerator::getParent(Id const &id)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mGraphicalModelApi.graphicalRepoApi().parent(mGraphicalModelApi.graphicalIdsByLogicalId(id).at(0));
	}

	return mGraphicalModelApi.graphicalRepoApi().parent(id);
}

IdList DatabasesGenerator::getChildren(Id const &id)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mGraphicalModelApi.graphicalRepoApi().children(mGraphicalModelApi.graphicalIdsByLogicalId(id).at(0));
	}

	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

IdList DatabasesGenerator::getBoundedWithOneToOneRealationship(Id const &id)
{
	IdList relationshipsOut = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
	IdList boundedEntities;
	boundedEntities.clear();
	boundedEntities.append(id);

	for (Id const &relationship : relationshipsOut) {
		if (mPassedElements.indexOf(relationship) == -1) {
			QString relationshipName = getProperty(relationship, "name").toByteArray();
			if (relationshipName == "One-to-one") {
				mPassedElements.append(relationship);
				Id newEntity = mLogicalModelApi.logicalRepoApi().to(relationship);
				mPassedElements.append(newEntity);
				boundedEntities.append(getBoundedWithOneToOneRealationship(newEntity));
			}
		}
	}

	IdList relationshipsIn = mLogicalModelApi.logicalRepoApi().incomingLinks(id);
	for (Id const &relationship : relationshipsIn) {
		if (mPassedElements.indexOf(relationship) == -1) {
			QString relationshipName = getProperty(relationship, "name").toByteArray();
			if (relationshipName == "One-to-one") {
				mPassedElements.append(relationship);
				Id newEntity = mLogicalModelApi.logicalRepoApi().from(relationship);
				mPassedElements.append(newEntity);
				boundedEntities.append(getBoundedWithOneToOneRealationship(newEntity));
			}
		}
	}
	return boundedEntities;
}

void DatabasesGenerator::error(QString const &error, bool isCritical)
{
	if (isCritical) {
		mInterpretersInterface.errorReporter()->addCritical(error);
	} else {
		mInterpretersInterface.errorReporter()->addWarning(error, Id::rootId());
	}
	mCurrentDiagram = Id::rootId();
}

Id DatabasesGenerator::getPrimaryKey(Id const &entityId)
{
	IdList attributesSet = getChildren(entityId);
	if (attributesSet.isEmpty()) {
		return Id::rootId();
	}
	Id primaryKey = Id::rootId();
	int primaryKeyCount = 0;
	for (Id const &attributeId : attributesSet) {
		bool isPrimaryKey = getProperty(attributeId, "isPrimaryKey").toBool();
		if (isPrimaryKey) {
			primaryKey = attributeId;
			primaryKeyCount++;
		}
	}
	if (primaryKeyCount != 1) {
		error(
			"Invalid number of primary key in entity with name '"
			+ getProperty(entityId, "Name").toString()
			+  "': "
			+ QString::number(primaryKeyCount)
			, true
		);
		return Id::rootId();
	}
	return primaryKey;
}

Id DatabasesGenerator::getPrimaryKeyOfSet(IdList const &entitySet)
{
	return getPrimaryKey(entitySet.at(0));
}
QString DatabasesGenerator::getPrimaryKeyNameOfSet(IdList const &entitySet)
{
	return("PrimaryKeyOfTable" + getProperty(entitySet.at(0), "Name").toByteArray());
}

int DatabasesGenerator::getParentNumber(Id const &childEntity, QList<IdList> const &set)
{
	int listCounter = 0;
	for (IdList const &list : set) {
		if (list.indexOf(childEntity) != -1)
			return listCounter;
		listCounter++;
	}
	return -1;
}

QString DatabasesGenerator::getListTableName(IdList const &list)
{
	QString name = "";
	bool isFirst = true;
	for (Id const &id : list) {
		if (!isFirst) {
			name += "And";
		} else {
			isFirst = false;
		}
		name += getProperty(id, "Name").toByteArray();
	}
	return name;
}

bool DatabasesGenerator::checkEntities()
{
	bool result = true;
	IdList entities = findNodes("Entity");
	for (Id const &entity : entities) {
		QString name = getProperty(entity, "Name").toString();
		if (name.isEmpty()) {
			result = false;
			error(tr("Entity has no name"), true);
		}
		IdList attributes = getChildren(entity);
		if (attributes.isEmpty()) {
			error(tr("Entity with name '") + name + tr("' has no attributes"), false);
		}
	}
	return result;
}

bool DatabasesGenerator::checkAttributes()
{
	bool result = true;
	IdList attributes = findNodes("Attribute");
	for (Id const &attribute : attributes) {
		QString name = getProperty(attribute, "Name").toString();
		QString datatype = getProperty(attribute, "DataType").toString();
		Id parent = getParent(attribute);
		QString parentName = getProperty(parent, "Name").toString();
		if (name.isEmpty()) {
			result = false;
			error(tr("Attribute has no name in entity '") + parentName + "'", true);
		}
		if (datatype.isEmpty()) {
			result = false;
			error(tr("Attribute has no datatype in entity '") + parentName + "'", true);
		}
	}
	return result;
}

bool DatabasesGenerator::checkRelationships()
{
	bool result = true;
	IdList relationships = findNodes("OneToManyRelationship");
	relationships.append(findNodes("ManyToManyRelationship"));
	relationships.append(findNodes("OneToOneRelationship"));
	for (Id const &relationship : relationships) {
		Id out = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		if (out == Id::rootId() || from == Id::rootId()) {
			result = false;
			error(getProperty(relationship, "name").toString()
				+ tr(" relationship with name '")
				+ getProperty(relationship, "Name").toString()
				+ tr("' has invalid ends"), true);
		}
	}
	return result;
}

bool DatabasesGenerator::checkCorrectness()
{
	mErrorReporter->clearErrors();
	bool checkOne = checkRelationships();
	bool checkTwo = checkAttributes();
	bool checkThree = checkEntities();
	if (checkOne && checkTwo && checkThree) {
		mErrorReporter->addInformation("Diagram is correct.");
	}
	return checkOne && checkTwo && checkThree;
}

qReal::Id DatabasesGenerator::createElementFromString(QString const &elemName
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

qReal::Id DatabasesGenerator::makeColumnFromAttribute(Id const &attributeId, Id const &parentId)
{
	mGraphicalModelApi.position(attributeId);
	Id logicalColumnId = createElementFromString("Column", QPointF(), parentId);
	QString rowName = getProperty(attributeId, "Name").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalColumnId, rowName, "Name");
	return logicalColumnId;
}

qReal::Id DatabasesGenerator::makeTableFromEntity(Id const &entityId, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(entityId).first());
	Id logicalTableId = createElementFromString("Table", coord, parentId);
	QString tableName = getProperty(entityId, "Name").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "Name");

	IdList attributesSet = getChildren(entityId);
	for (Id const &attributeId : attributesSet) {
		makeColumnFromAttribute(attributeId, logicalTableId);
	}

	IdList tableNodes = findNodes("Table");
	return logicalTableId;
}

qReal::Id DatabasesGenerator::makeTableFromEntitySet(IdList const &set, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(set.first()).first());
	Id logicalTableId = createElementFromString("Table", coord, parentId);
	QString tableName = getListTableName(set);
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "Name");

	for (Id const &entityId : set){
		IdList attributes = getChildren(entityId);
		for (Id const &attributeId : attributes) {
			makeColumnFromAttribute(attributeId, logicalTableId);
		}
	}
	return logicalTableId;
}

qReal::Id DatabasesGenerator::copyOneToManyRelationship(Id const &logicalModelId
		, Id const &parentId
		, Id const &from
		, Id const &to)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(logicalModelId).first());
	Id logicalId = createElementFromString("PhysicalOneToManyRelationship", coord, parentId);
	Id graphicalId = mGraphicalModelApi.graphicalIdsByLogicalId(logicalId).first();

	QString name = getProperty(logicalModelId, "Name").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalId, name, "Name");

	mLogicalModelApi.setFrom(logicalId, from);
	mLogicalModelApi.setTo(logicalId, to);
	mGraphicalModelApi.setFrom(graphicalId, mGraphicalModelApi.graphicalIdsByLogicalId(from).first());
	mGraphicalModelApi.setTo(graphicalId,  mGraphicalModelApi.graphicalIdsByLogicalId(to).first());
	return logicalId;
}

qReal::Id DatabasesGenerator::copyManyToManyRelationship(Id const &logicalModelId
		, Id const &parentId
		, Id const &from
		, Id const &to)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(logicalModelId).first());
	Id logicalId = createElementFromString("PhysicalManyToManyRelationship", coord, parentId);
	Id graphicalId = mGraphicalModelApi.graphicalIdsByLogicalId(logicalId).first();

	QString name = getProperty(logicalModelId, "Name").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalId, name, "Name");

	mLogicalModelApi.setFrom(logicalId, from);
	mLogicalModelApi.setTo(logicalId, to);
	mGraphicalModelApi.setFrom(graphicalId, mGraphicalModelApi.graphicalIdsByLogicalId(from).first());
	mGraphicalModelApi.setTo(graphicalId,  mGraphicalModelApi.graphicalIdsByLogicalId(to).first());
	return logicalId;
}

QList<IdList> DatabasesGenerator::processEntities(Id const &logicalDiagramId)
{
	QList<IdList> oneToOneBoundedEntitiesSets;
	mPassedElements.clear();
	IdList entityNodes = findNodes("Entity");
	for (Id const &entityId : entityNodes) {
		IdList relationships = mLogicalModelApi.logicalRepoApi().outgoingLinks(entityId);
		relationships.append(mLogicalModelApi.logicalRepoApi().incomingLinks(entityId));
		// If alone entity
		// make table
		if (relationships.isEmpty()) {
			mPassedElements.append(entityId);
			makeTableFromEntity(entityId, logicalDiagramId);
		}
		else {
		// if not alone entity
		// make set
			if (!mPassedElements.contains(entityId)) {
				mPassedElements.append(entityId);
				// List for set of tables bounded with one-to-one relationship
				IdList oneToOneBoundedEntitySet;
				oneToOneBoundedEntitySet.clear();
				oneToOneBoundedEntitySet = getBoundedWithOneToOneRealationship(entityId);
				oneToOneBoundedEntitiesSets.append(oneToOneBoundedEntitySet);
			}
		}
	}
	return oneToOneBoundedEntitiesSets;
}

bool DatabasesGenerator::processOneToManyRelationships(QList<IdList> const &oneToOneBoundedEntitiesSets
		, IdList setTables
		, Id const &logicalDiagramId)
{
	IdList oneToManyRelationships = findNodes("OneToManyRelationship");
	for (Id const &relationship : oneToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentNumber(to, oneToOneBoundedEntitiesSets);
		int fromSet = getParentNumber(from, oneToOneBoundedEntitiesSets);

		if (mRelMatrix[toSet][fromSet] != 0) {
			error("Too many relationships from "
				 + getProperty(from, "Name").toString()
				 + "to " + getProperty(to, "Name").toString(), true);
			return false;
		}

		QString columnNameForRelationship = getProperty(relationship, "ColumnName").toString();
		if (columnNameForRelationship.isEmpty()) {
			mErrorReporter->addInformation(tr("Column name for one-to-many relationship with name '")
				+ getProperty(relationship, "Name").toString()
				+ tr("' is`n specified. Column name was generated automatically."));
			columnNameForRelationship = getPrimaryKeyNameOfSet(oneToOneBoundedEntitiesSets.at(toSet));
		}
		mRelMatrix[fromSet][toSet] = 1;
		mRelMatrix[toSet][fromSet] = -1;

		// add bounding attribute
		Id logicalColumnId = createElementFromString("Column", QPointF(), setTables.at(fromSet), true);
		QString rowName = getProperty(relationship, "ColumnName").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalColumnId, rowName, "Name");
		// copy relationship
		copyOneToManyRelationship(relationship, logicalDiagramId, setTables.at(fromSet), setTables.at(toSet));
	}
	return true;
}

bool DatabasesGenerator::processManyToManyRelationships(QList<IdList> const &oneToOneBoundedEntitiesSets
		, IdList setTables
		, Id const &logicalDiagramId)
{
	IdList manyToManyRelationships = findNodes("ManyToManyRelationship");
	for (Id const &relationship : manyToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentNumber(to, oneToOneBoundedEntitiesSets);
		int fromSet = getParentNumber(from, oneToOneBoundedEntitiesSets);

		if (mRelMatrix[toSet][fromSet] != 0) {
			error(tr("Too many relationships from ")
				+ getProperty(from, "Name").toString()
				+ tr(" to ") + getProperty(to, "Name").toString(), true);
			return false;
		}

		QString relationshipTableName = getProperty(relationship, "TableName").toString();
		if (relationshipTableName.isEmpty()) {
			mErrorReporter->addInformation(tr("Table name for many-to-many relationship with name '")
				+ getProperty(relationship, "Name").toString()
				+ tr("' is`t specified. Table name was generated automatically"), Id::rootId());
			relationshipTableName = (getListTableName(oneToOneBoundedEntitiesSets.at(fromSet))
				+ "_" + getListTableName(oneToOneBoundedEntitiesSets.at(toSet))).toUtf8();
		}
		mRelMatrix[toSet][fromSet] = 2;
		mRelMatrix[toSet][fromSet] = 2;

		// Creating
		Id logicalTableId = createElementFromString("Table", QPointF(), logicalDiagramId);
		mLogicalModelApi.setPropertyByRoleName(logicalTableId, relationshipTableName, "Name");

		//copy relationship
		copyManyToManyRelationship(relationship
				, logicalDiagramId
				, setTables.at(fromSet)
				, logicalTableId);
		copyManyToManyRelationship(relationship
				, logicalDiagramId
				, logicalTableId
				, setTables.at(toSet));

		//add bounding attribute
		Id logicalColumnIdOne = createElementFromString("Column", QPointF(), logicalTableId, true);
		QString rowName = "keyOf" + getProperty(setTables.at(fromSet), "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalColumnIdOne, rowName, "Name");

		//add bounding attribute 2
		Id logicalColumnIdTwo = createElementFromString("Column", QPointF(), logicalTableId, true);
		QString rowName2 = "keyOf" + getProperty(setTables.at(toSet), "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalColumnIdTwo, rowName2, "Name");
	}
	return true;
}

void DatabasesGenerator::generatePhysicalModel()
{
	clearPhysicalModel();
	mErrorReporter->clear();

	if (!checkCorrectness()) {
		return;
	}

	Id logicalDiagramId = createElementFromString("DatabasesPhysicalNode");

	QList<IdList> oneToOneBoundedEntitiesSets = processEntities(logicalDiagramId);
	IdList setTables;
	for (IdList const &set : oneToOneBoundedEntitiesSets) {
		Id logicalTableId = makeTableFromEntitySet(set, logicalDiagramId);
		setTables.append(logicalTableId);
	}

	// mRelMatrix[i][k] =
	// 0 - no relationships
	// 1 - one-to-many relationship (i - many, k - one)
	// -1 - one-to-many relationship (i - one, k - many)
	// 2 - many-to-many relationship
	// 3 - one-to-one relationship
	int oneToOneSetsSize = oneToOneBoundedEntitiesSets.size();
	mRelMatrix = new int*[oneToOneSetsSize];
	for (int i = 0; i < oneToOneSetsSize; i++) {
		mRelMatrix[i] = new int[oneToOneSetsSize];
		for (int k = 0; k < oneToOneSetsSize; k++)
			mRelMatrix[i][k] = 0;
		mRelMatrix[i][i] = 3;
	}

	if (!processOneToManyRelationships(oneToOneBoundedEntitiesSets
			, setTables
			, logicalDiagramId))
		return;
	if (!processManyToManyRelationships(oneToOneBoundedEntitiesSets
			, setTables
			, logicalDiagramId))
		return;

	for (int i = 0; i < oneToOneSetsSize; i++) {
		delete[] mRelMatrix[i];
	}
	delete[] mRelMatrix;
	mErrorReporter->addInformation(tr("Physical model was generated successfully"));
}

void DatabasesGenerator::generateSQLCode()
{
	mErrorReporter->clear();

	QString codeFileName = mPreferencesPage->getCodeGenerationFilename();
	codeFileName = QString("C:/Coursework/text.txt");
	if (codeFileName.isEmpty()) {
		mErrorReporter->addError(QString("Code file name is empty. Check preferences"));
		return;
	}
	codeFile.setFileName(codeFileName);
	if (!codeFile.open(QIODevice::WriteOnly)) {
		mErrorReporter->addError(QString("File didn't open"));
		return;
	}

	mCodeGenerationMode = mPreferencesPage->getCodeGenerationMode(); // update mode

	if (mDbms == "SqlServer2008")
		generateWithSqlServer2008();
	else if (mDbms == "MySql5")
		generateWithMySql5();
	else if (mDbms == "Sqlite")
		generateWithSqlite();
	else if (mDbms == "MicrosoftAccess")
		generateWithMicrosoftAccess();
	else if (mDbms == "PostgreSql")
		generateWithPostgreSql();

	codeFile.close();
	mErrorReporter->addInformation(tr("Code was generated successfully"));

	QString codeFileNameForEditor;
	int strSize = codeFileName.size();
	for (int i = 0; i < strSize; i++) {
		QChar ch = codeFileName.at(i);
		if (ch != QChar('/')) {
			codeFileNameForEditor.append(ch);
		}
		else {
			codeFileNameForEditor.append(QString("\\"));
		}
	}
	// TODO: file opening for sql-code or window for sql code
	/*if (mPreferencesPage->needToOpenFileAfterGeneration()) {
		// Windows
		QProcess *proc = new QProcess();
		proc->start("explorer C:\\Coursework\\qreal\\bin\\debug\\code.txt");
		// proc->start("see /home/alex/Report.doc");
	}*/

}

void DatabasesGenerator::generateWithSqlServer2008()
{
	if (mCodeGenerationMode == CreateTable)
		createTableModeWithSqlServer2008();
	else if (mCodeGenerationMode == AlterTable)
		alterTableModeWithSqlServer2008();
}

void DatabasesGenerator::generateWithMySql5()
{
	if (mCodeGenerationMode == CreateTable)
		createTableModeWithMySql5();
	else if (mCodeGenerationMode == AlterTable)
		alterTableModeWithMySql5();
}

void DatabasesGenerator::generateWithSqlite()
{
	if (mCodeGenerationMode == CreateTable)
		createTableModeWithSqlite();
	else if (mCodeGenerationMode == AlterTable)
		alterTableModeWithSqlite();
}

void DatabasesGenerator::generateWithMicrosoftAccess()
{
	if (mCodeGenerationMode == CreateTable)
		createTableModeWithMicrosoftAccess();
	else if (mCodeGenerationMode == AlterTable)
		alterTableModeWithMicrosoftAccess();
}

void DatabasesGenerator::generateWithPostgreSql()
{
	if (mCodeGenerationMode == CreateTable)
		createTableModeWithPostgreSql();
	else if (mCodeGenerationMode == AlterTable)
		alterTableModeWithPostgreSql();
}

void DatabasesGenerator::createTableModeWithSqlServer2008()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("CREATE TABLE ");

		codeFile.write("dbo." + getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool())
				codeFile.write(" NULL");
			else if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "isPrimaryKey").toBool())
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");
		}
		codeFile.write("\r\n);\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithMySql5()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("CREATE ");

		if (getProperty(tableId, "temporary").toBool())
			codeFile.write("TEMPORARY ");

		codeFile.write("TABLE ");

		if (getProperty(tableId, "if_not_exists").toBool())
			codeFile.write("IF NOT EXISTS ");

		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool())
					codeFile.write(" NULL");
				else if (getProperty(rowId, "notNull").toBool())
					codeFile.write(" NOT NULL");

				if (getProperty(rowId, "isPrimaryKey").toBool())
					codeFile.write(" PRIMARY KEY");

				if (getProperty(rowId, "unique").toBool())
					codeFile.write(" UNIQUE");

				QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
				if (!defaultValue.isEmpty())
					codeFile.write(" DEFAULT '" + defaultValue + "'");

				if (getProperty(rowId, "auto_increment").toBool())
					codeFile.write(" AUTO_INCREMENT");
		}
		codeFile.write("\r\n);\r\n\r\n");

		QByteArray tableType = getProperty(tableId, "type").toByteArray();
		if (!tableType.isEmpty())
			codeFile.write(" TYPE " + tableType);

		QByteArray autoIncrementValue = getProperty(tableId, "auto_increment").toByteArray();
		if (!autoIncrementValue.isEmpty())
			codeFile.write(" AUTO_INCREMENT " + autoIncrementValue);

		QByteArray avgColumnLength = getProperty(tableId, "avg_row_length").toByteArray();
		if (!avgColumnLength.isEmpty())
			codeFile.write(" AVG_ROW_LENGTH " + avgColumnLength);

		if (getProperty(tableId, "check_sum").toBool())
			codeFile.write(" CHECKSUM");

//		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty())
			codeFile.write(" COMMENT " + comment);

		QByteArray maxColumns = getProperty(tableId, "max_rows").toByteArray();
		if (!maxColumns.isEmpty())
			codeFile.write(" MAX_ROWS " + maxColumns);

		QByteArray minColumns = getProperty(tableId, "min_rows").toByteArray();
		if (!minColumns.isEmpty())
			codeFile.write(" MIN_ROWS " + minColumns);

		if (getProperty(tableId, "pack_keys").toBool())
			codeFile.write(" PACK_KEYS");

		if (getProperty(tableId, "delay_key_write").toBool())
			codeFile.write(" DELAY_KEY_WRITE");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithSqlite()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("CREATE ");

		if (getProperty(tableId, "temporary").toBool())
			codeFile.write("TEMPORARY ");
		else if (getProperty(tableId, "temp").toBool())
			codeFile.write("TEMP ");

		codeFile.write("TABLE ");

		if (getProperty(tableId, "if_not_exists").toBool())
			codeFile.write("IF NOT EXISTS ");

		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool())
				codeFile.write(" NULL");
			else if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "isPrimaryKey").toBool())
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");

			QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
			if (!defaultValue.isEmpty())
				codeFile.write(" DEFAULT '" + defaultValue + "'");

			if (getProperty(rowId, "auto_increment").toBool())
				codeFile.write(" AUTO_INCREMENT");
		}
		codeFile.write("\r\n);\r\n\r\n");

		if (getProperty(tableId, "without_rowid").toBool())
			codeFile.write(" WITHOUT ROWID");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithMicrosoftAccess()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("CREATE ");

		if (getProperty(tableId, "temporary").toBool())
			codeFile.write("TEMPORARY ");

		codeFile.write("TABLE ");

		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "isPrimaryKey").toBool())
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");

			if (getProperty(rowId, "with_compression").toBool())
				codeFile.write(" WITH COMPRESSION");
			else if (getProperty(rowId, "with_comp").toBool())
				codeFile.write(" WITH COMP");

		}
		codeFile.write("\r\n);\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithPostgreSql()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("CREATE ");

		if (getProperty(tableId, "global").toBool())
			codeFile.write("GLOBAL ");
		else if (getProperty(tableId, "local").toBool())
			codeFile.write("LOCAL ");

		if (getProperty(tableId, "temporary").toBool())
			codeFile.write("TEMPORARY ");
		else if (getProperty(tableId, "temp").toBool())
			codeFile.write("TEMP ");

		if (getProperty(tableId, "unlogged").toBool())
			codeFile.write("UNLOGGED ");

		codeFile.write("TABLE ");

		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "null").toBool())
				codeFile.write(" NULL");

			QByteArray checkExpression = getProperty(rowId, "check").toByteArray();
			if (!checkExpression.isEmpty()) {
				codeFile.write(" CHECK");
				codeFile.write(checkExpression);
				codeFile.write(" ");
			}

			QByteArray defaultValue = getProperty(rowId, "default").toByteArray();
			if (!defaultValue.isEmpty()) {
				codeFile.write(" DEFAULT");
				codeFile.write(defaultValue);
				codeFile.write(" ");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool());
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");

		}

		QByteArray inherits = getProperty(tableId, "inherits").toByteArray();
		if (!inherits.isEmpty()) {
			codeFile.write(" INHERITS ");
			codeFile.write(inherits);
			codeFile.write(" ");
		}

		QByteArray with = getProperty(tableId, "with").toByteArray();
		if (!inherits.isEmpty()) {
			codeFile.write(" WITH ");
			codeFile.write(with);
			codeFile.write(" ");
		}
		else if (getProperty(tableId, "with_oids").toBool()) {
			codeFile.write(" WITH OIDS ");
		}
		else if (getProperty(tableId, "without_oids").toBool()) {
			codeFile.write(" WITHOUT OIDS ");
		}

		QByteArray onCommit = getProperty(tableId, "on_commit").toByteArray();
		if (!inherits.isEmpty()) {
			codeFile.write(" ON COMMIT ");
			codeFile.write(onCommit);
			codeFile.write(" ");
		}

		QByteArray tablespace = getProperty(tableId, "tablespace").toByteArray();
		if (!inherits.isEmpty()) {
			codeFile.write(" TABLESPACE ");
			codeFile.write(tablespace);
			codeFile.write(" ");
		}

		codeFile.write("\r\n);\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithSqlServer2008()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("ALTER TABLE ");
		codeFile.write(getProperty(tableId, "Name").toByteArray());
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write("ADD COLUMN (");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool())
				codeFile.write(" NULL");
			else if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "isPrimaryKey").toBool())
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");

			codeFile.write(")");
		}
		codeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithMySql5()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("ALTER TABLE ");
		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write("ADD COLUMN (");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool())
					codeFile.write(" NULL");
				else if (getProperty(rowId, "notNull").toBool())
					codeFile.write(" NOT NULL");

				if (getProperty(rowId, "isPrimaryKey").toBool())
					codeFile.write(" PRIMARY KEY");

				if (getProperty(rowId, "unique").toBool())
					codeFile.write(" UNIQUE");

				QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
				if (!defaultValue.isEmpty())
					codeFile.write(" DEFAULT '" + defaultValue + "'");

				if (getProperty(rowId, "auto_increment").toBool())
					codeFile.write(" AUTO_INCREMENT");

				codeFile.write(")");
		}
		codeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithSqlite()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("ALTER TABLE");
		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write("ADD COLUMN (");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool())
				codeFile.write(" NULL");
			else if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "isPrimaryKey").toBool())
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");

			QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
			if (!defaultValue.isEmpty())
				codeFile.write(" DEFAULT '" + defaultValue + "'");

			if (getProperty(rowId, "auto_increment").toBool())
				codeFile.write(" AUTO_INCREMENT");

			codeFile.write(")");
		}
		codeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithMicrosoftAccess()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("ALTER TABLE ");
		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write("ADD COLUMN (");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "isPrimaryKey").toBool())
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");

			if (getProperty(rowId, "with_compression").toBool())
				codeFile.write(" WITH COMPRESSION");
			else if (getProperty(rowId, "with_comp").toBool())
				codeFile.write(" WITH COMP");

			codeFile.write(")");
		}
		codeFile.write(";\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithPostgreSql()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			codeFile.write("-- ");
			codeFile.write(comment);
			codeFile.write("\r\n");
		}

		codeFile.write("ALTER TABLE ");
		codeFile.write(getProperty(tableId, "Name").toByteArray());
		codeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write("ADD COLUMN (");
			codeFile.write(getProperty(rowId, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool())
				codeFile.write(" NOT NULL");

			if (getProperty(rowId, "null").toBool())
				codeFile.write(" NULL");

			QByteArray checkExpression = getProperty(rowId, "check").toByteArray();
			if (!checkExpression.isEmpty()) {
				codeFile.write(" CHECK");
				codeFile.write(checkExpression);
				codeFile.write(" ");
			}

			QByteArray defaultValue = getProperty(rowId, "default").toByteArray();
			if (!defaultValue.isEmpty()) {
				codeFile.write(" DEFAULT");
				codeFile.write(defaultValue);
				codeFile.write(" ");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool());
				codeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				codeFile.write(" UNIQUE");
			codeFile.write(")");
		}

		codeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			codeFile.write(query);
			codeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::changeEditor(QString const &dbmsName)
{
	mDbms = dbmsName;
}

}
}
