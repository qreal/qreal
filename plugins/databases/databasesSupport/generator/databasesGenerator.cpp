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

#include "databasesGenerator.h"

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>
#include <qrgui/models/elementInfo.h>

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
	, mCodeGenerationMode(CodeGenerationMode::CreateTable)
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

IdList DatabasesGenerator::findIndexes()
{
	IdList const children = mLogicalModelApi.logicalRepoApi().children(Id::rootId());
	IdList filteredChildren;
	for (Id const &child : children) {
		bool isLogicalElem = mLogicalModelApi.logicalRepoApi().isLogicalElement(child);
		if (child.element() == "Index" && child.editor() == mDbms && isLogicalElem) {
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
	IdList children;
	if (mLogicalModelApi.isLogicalId(id)) {
		children = mGraphicalModelApi.graphicalRepoApi().children(mGraphicalModelApi.graphicalIdsByLogicalId(id).at(0));
	} else {
		children = mGraphicalModelApi.graphicalRepoApi().children(id);
	}

	return children;
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

void DatabasesGenerator::error(QString const &error, bool isCritical, Id const &position)
{
	if (isCritical) {
		mInterpretersInterface.errorReporter()->addCritical(error, position);
	} else {
		mInterpretersInterface.errorReporter()->addWarning(error, position);
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
			+ getProperty(entityId, "entityName").toString()
			+  "': "
			+ QString::number(primaryKeyCount)
			, true
			, entityId
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
	return("PrimaryKeyOfTable" + getProperty(entitySet.at(0), "entityName").toByteArray());
}

int DatabasesGenerator::getParentNumber(Id const &childEntity, QList<IdList> const &set)
{
	int listCounter = 0;
	for (IdList const &list : set) {
		if (list.indexOf(childEntity) != -1) {
			return listCounter;
		}
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
		name += getProperty(id, "entityName").toByteArray();
	}
	return name;
}

bool DatabasesGenerator::checkEntities()
{
	bool result = true;
	IdList entities = findNodes("Entity");
	for (Id const &entity : entities) {
		QString name = getProperty(entity, "entityName").toString();
		if (name.isEmpty()) {
			result = false;
			error(tr("Entity has no name."), true, entity);
		}
		IdList attributes = getChildren(entity);
		if (attributes.isEmpty()) {
			error(tr("Entity with name '") + name + tr("' has no attributes."), false, entity);
		}
	}
	return result;
}

bool DatabasesGenerator::checkAttributes()
{
	bool result = true;
	IdList attributes = findNodes("Attribute");
	for (Id const &attribute : attributes) {
		QString name = getProperty(attribute, "attributeName").toString();
		QString datatype = getProperty(attribute, "DataType").toString();
		Id parent = getParent(attribute);
		QString parentName = getProperty(parent, "entityName").toString();
		if (name.isEmpty()) {
			result = false;
			error(tr("Attribute has no name in entity '") + parentName + "'.", true, parent);
		}
		if (datatype.isEmpty()) {
			result = false;
			error(tr("Attribute has no datatype in entity '") + parentName + "'.", true, parent);
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
			error(tr("Relationship with name '")
				+ getProperty(relationship, "Name").toString()
				+ tr("' has invalid ends."), true, relationship);
		}
	}
	return result;
}

bool DatabasesGenerator::checkTables()
{
	bool result = true;
	IdList tables = findNodes("Table");
	for (Id const &table : tables) {
		QString name = getProperty(table, "tableName").toString();
		if (name.isEmpty()) {
			result = false;
			error(tr("Table has no name."), true, table);
		}
		IdList columns = getChildren(table);
		if (columns.isEmpty()) {
			error(tr("Table with name '") + name + tr("' has no columns."), false, table);
		}
	}
	return result;
}

bool DatabasesGenerator::checkColumns()
{
	bool result = true;
	IdList columns = findNodes("Column");
	for (Id const &column : columns) {
		QString name = getProperty(column, "columnName").toString();
		QString datatype = getProperty(column, "DataType").toString();
		Id parent = getParent(column);
		QString parentName = getProperty(parent, "tableName").toString();
		if (name.isEmpty()) {
			result = false;
			error(tr("Column has no name in table '") + parentName + "'.", true, parent);
		}
		if (datatype.isEmpty()) {
			result = false;
			error(tr("Column has no datatype in table '") + parentName + "'.", true, parent);
		}
	}
	return result;
}

bool DatabasesGenerator::checkLogicalModelCorrectness()
{
	mErrorReporter->clearErrors();
	bool checkOne = checkRelationships();
	bool checkTwo = checkAttributes();
	bool checkThree = checkEntities();
	if (checkOne && checkTwo && checkThree) {
		mErrorReporter->addInformation(tr("Logical diagram is correct."));
	}
	return checkOne && checkTwo && checkThree;
}

bool DatabasesGenerator::checkPhysicalModelCorrectness()
{
	mErrorReporter->clearErrors();
	bool checkOne = checkTables();
	bool checkTwo = checkColumns();
	if (checkOne && checkTwo) {
		mErrorReporter->addInformation(tr("Physical diagram is correct."));
	}
	return checkOne && checkTwo;
}

qReal::Id DatabasesGenerator::createElementFromString(QString const &elemName
		, QPointF coord
		, Id const &parentLogicalId
		, bool coordByParent)
{
	const Id id = Id::loadFromString(QString("qrm:/" + mDbms
			+ "/DatabasesPhysicalModelMetamodel/" + elemName));

	Id logicalId = mLogicalModelApi.createElement(Id::rootId(), id);
	Id graphicalParentId = Id::rootId();
	if (parentLogicalId != Id::rootId()) {
		graphicalParentId = mGraphicalModelApi.graphicalIdsByLogicalId(parentLogicalId).first();
		if (coordByParent) {
			coord = mGraphicalModelApi.position(graphicalParentId);
		}
	}

	ElementInfo info(logicalId, logicalId, parentLogicalId, graphicalParentId
				, {{"name", elemName}}, {{"position", coord}}, Id()
				, mLogicalModelApi.editorManagerInterface().isNodeOrEdge(id.editor(), id.element()) < 0);
	mGraphicalModelApi.createElements(QList<ElementInfo>() << info);
	return logicalId;
}

qReal::Id DatabasesGenerator::makeColumnFromAttribute(Id const &attributeId, Id const &parentId)
{
	mGraphicalModelApi.position(attributeId);
	Id logicalColumnId = createElementFromString("Column", QPointF(), parentId);
	QString rowName = getProperty(attributeId, "attributeName").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalColumnId, rowName, "columnName");


	bool primaryKey = getProperty(attributeId, "isPrimaryKey").toBool();
	if (primaryKey) {
		mLogicalModelApi.setPropertyByRoleName(logicalColumnId, true, "isPrimaryKey");
	}

	bool unique = getProperty(attributeId, "isUnique").toBool();
	if (unique) {
		mLogicalModelApi.setPropertyByRoleName(logicalColumnId, true, "isUnique");
	}

	bool notNull = getProperty(attributeId, "notNull").toBool();
	if (notNull) {
		mLogicalModelApi.setPropertyByRoleName(logicalColumnId, true, "notNull");
	}

	QString dataType = getProperty(attributeId, "DataType").toString();
	if (!dataType.isEmpty()) {
		mLogicalModelApi.setPropertyByRoleName(logicalColumnId, dataType, "DataType");
	}
	return logicalColumnId;
}

qReal::Id DatabasesGenerator::makeTableFromEntity(Id const &entityId, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(entityId).first());
	Id logicalTableId = createElementFromString("Table", coord, parentId, false);
	QString tableName = getProperty(entityId, "entityName").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "tableName");

	IdList attributesSet = getChildren(entityId);
	for (Id const &attributeId : attributesSet) {
		makeColumnFromAttribute(attributeId, logicalTableId);
	}
	return logicalTableId;
}

qReal::Id DatabasesGenerator::makeTableFromEntitySet(IdList const &set, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(set.first()).first());
	Id logicalTableId = createElementFromString("Table", coord, parentId, false);
	QString tableName = getListTableName(set);
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "tableName");

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
	Id logicalId = createElementFromString("PhysicalOneToManyRelationship", coord, parentId, false);
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
	Id logicalId = createElementFromString("PhysicalManyToManyRelationship", coord, parentId, false);
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
		} else {
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
				 + getProperty(from, "entityName").toString()
				 + "to " + getProperty(to, "entityName").toString(), false, from);
		}

		QString columnNameForRelationship = getProperty(relationship, "columnName").toString();
		if (columnNameForRelationship.isEmpty()) {
			mErrorReporter->addInformation(tr("Column name for one-to-many relationship with name '")
				+ getProperty(relationship, "Name").toString()
				+ tr("' is not specified. Column name was generated automatically.")
				, relationship);
			columnNameForRelationship = getPrimaryKeyNameOfSet(oneToOneBoundedEntitiesSets.at(toSet));
		}
		mRelMatrix[fromSet][toSet] = 1;
		mRelMatrix[toSet][fromSet] = -1;

		// add bounding attribute
		Id logicalColumnId = createElementFromString("Column", QPointF(), setTables.at(fromSet));
		QString rowName = getProperty(relationship, "columnName").toString();
		if (rowName.isEmpty()) {
			rowName = "keyOf" + getProperty(to, "entityName").toString();
		}
		mLogicalModelApi.setPropertyByRoleName(logicalColumnId, rowName, "columnName");

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
				+ getProperty(from, "entityName").toString()
				+ tr(" to ") + getProperty(to, "entityName").toString() + tr("."), false, from);
		}

		QString relationshipTableName = getProperty(relationship, "tableName").toString();
		if (relationshipTableName.isEmpty()) {
			mErrorReporter->addInformation(tr("Table name for many-to-many relationship with name '")
				+ getProperty(relationship, "tableName").toString()
				+ tr("' is not specified. Table name was generated automatically.")
				, relationship);
			relationshipTableName = (getListTableName(oneToOneBoundedEntitiesSets.at(fromSet))
				+ "_" + getListTableName(oneToOneBoundedEntitiesSets.at(toSet))).toUtf8();
		}
		mRelMatrix[toSet][fromSet] = 2;
		mRelMatrix[fromSet][toSet] = 2;

		QPointF tableGraphicalIdFrom = mGraphicalModelApi.position(
					mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(fromSet)).first());
		QPointF tableGraphicalIdTo = mGraphicalModelApi.position(
					mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(toSet)).first());
		// Creating
		QPointF boundingTableCoord = QPointF((tableGraphicalIdFrom.x() + tableGraphicalIdTo.x()) / 2.0
				, (tableGraphicalIdFrom.y() + tableGraphicalIdTo.y()) / 2.0);
		Id logicalTableId = createElementFromString("Table", boundingTableCoord, logicalDiagramId, false);
		mLogicalModelApi.setPropertyByRoleName(logicalTableId, relationshipTableName, "tableName");

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
		Id logicalColumnIdOne = createElementFromString("Column", QPointF(), logicalTableId);
		QString rowName = "keyOf" + getProperty(setTables.at(fromSet), "tableName").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalColumnIdOne, rowName, "columnName");

		//add bounding attribute 2
		Id logicalColumnIdTwo = createElementFromString("Column", QPointF(), logicalTableId);
		QString rowName2 = "keyOf" + getProperty(setTables.at(toSet), "tableName").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalColumnIdTwo, rowName2, "columnName");
	}
	return true;
}

void DatabasesGenerator::generatePhysicalModel()
{
	clearPhysicalModel();
	mErrorReporter->clear();

	if (!checkLogicalModelCorrectness()) {
		return;
	}

	Id logicalDiagramId = createElementFromString("DatabasesPhysicalNode", QPointF(), Id::rootId(), false);

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
			, logicalDiagramId)) {
		return;
	}
	if (!processManyToManyRelationships(oneToOneBoundedEntitiesSets
			, setTables
			, logicalDiagramId)) {
		return;
	}

	for (int i = 0; i < oneToOneSetsSize; i++) {
		delete[] mRelMatrix[i];
	}
	delete[] mRelMatrix;
	mErrorReporter->addInformation(tr("Physical model was generated successfully."));
}

void DatabasesGenerator::generateSQLCode()
{
	mErrorReporter->clear();

	if (!checkPhysicalModelCorrectness()) {
		return;
	}

	QString mCodeFileName = mPreferencesPage->getCodeGenerationFilename();
	if (mCodeFileName.isEmpty()) {
		mErrorReporter->addError(tr("Code file name is empty. Check preferences."));
		return;
	}
	mCodeFile.setFileName(mCodeFileName);
	if (!mCodeFile.open(QIODevice::WriteOnly)) {
		mErrorReporter->addError(tr("File didn't open."));
		return;
	}

	mCodeGenerationMode = mPreferencesPage->getCodeGenerationMode(); // update mode

	if (mDbms == "SqlServer2008") {
		generateWithSqlServer2008();
	} else if (mDbms == "MySql5") {
		generateWithMySql5();
	} else if (mDbms == "Sqlite") {
		generateWithSqlite();
	} else if (mDbms == "MicrosoftAccess") {
		generateWithMicrosoftAccess();
	} else if (mDbms == "PostgreSql") {
		generateWithPostgreSql();
	}

	mCodeFile.close();
	mErrorReporter->addInformation(tr("Code was generated successfully."));

	QString mCodeFileNameForEditor;
	int strSize = mCodeFileName.size();
	for (int i = 0; i < strSize; i++) {
		QChar ch = mCodeFileName.at(i);
		if (ch != QChar('/')) {
			mCodeFileNameForEditor.append(ch);
		} else {
			mCodeFileNameForEditor.append(QString("\\"));
		}
	}

	// TODO: file opening for sql-code or window for sql code
}

void DatabasesGenerator::generateWithSqlServer2008()
{
	if (mCodeGenerationMode == CodeGenerationMode::CreateTable) {
		createTableModeWithSqlServer2008();
	} else if (mCodeGenerationMode == CodeGenerationMode::AlterTable) {
		alterTableModeWithSqlServer2008();
	}

	IdList indexes = findIndexes();

	for (Id const &index : indexes) {
		mCodeFile.write("\r\n");
		if (index.editor() == mDbms) {
			mCodeFile.write("CREATE ");

			bool isUnique = mLogicalModelApi.logicalRepoApi().property(index, "isUnique").toBool();
			if (isUnique) {
				mCodeFile.write("UNIQUE ");
			}

			bool clustered = mLogicalModelApi.logicalRepoApi().property(index, "clustered").toBool();
			if (clustered) {
				mCodeFile.write("CLUSTERED ");
			} else {
				mCodeFile.write("NONCLUSTERED ");
			}

			mCodeFile.write("INDEX ");

			QByteArray name = mLogicalModelApi.logicalRepoApi().property(index, "indexName").toByteArray();
			mCodeFile.write(name);

			mCodeFile.write(" ON ");

			Id tableId = Id::loadFromString(mLogicalModelApi.logicalRepoApi().property(index, "tableName").toString());
			QByteArray tableName = getProperty(tableId, "tableName").toByteArray();
			mCodeFile.write(tableName);

			QByteArray columns = mLogicalModelApi.logicalRepoApi().property(index, "columnNames").toByteArray();
			mCodeFile.write(" (");
			mCodeFile.write(columns);
			mCodeFile.write(") ");
		}
	}
}

void DatabasesGenerator::generateWithMySql5()
{
	if (mCodeGenerationMode == CodeGenerationMode::CreateTable) {
		createTableModeWithMySql5();
	} else if (mCodeGenerationMode == CodeGenerationMode::AlterTable) {
		alterTableModeWithMySql5();
	}

	IdList indexes = findIndexes();

	for (Id const &index : indexes) {
		mCodeFile.write("\r\n");
		if (index.editor() == mDbms) {
			mCodeFile.write("CREATE ");

			bool isUnique = mLogicalModelApi.logicalRepoApi().property(index, "isUnique").toBool();
			if (isUnique) {
				mCodeFile.write("UNIQUE ");
			} else {
				bool fulltext = mLogicalModelApi.logicalRepoApi().property(index, "fulltext").toBool();
				if (fulltext) {
					mCodeFile.write("FULLTEXT ");
				}
			}

			mCodeFile.write("INDEX ");

			QByteArray name = mLogicalModelApi.logicalRepoApi().property(index, "indexName").toByteArray();
			mCodeFile.write(name);

			mCodeFile.write(" ON ");

			Id tableId = Id::loadFromString(mLogicalModelApi.logicalRepoApi().property(index, "tableName").toString());
			QByteArray tableName = getProperty(tableId, "tableName").toByteArray();
			mCodeFile.write(tableName);

			QByteArray columns = mLogicalModelApi.logicalRepoApi().property(index, "columnNames").toByteArray();
			mCodeFile.write(" (");
			mCodeFile.write(columns);
			mCodeFile.write(") ");
		}
	}
}

void DatabasesGenerator::generateWithSqlite()
{
	if (mCodeGenerationMode == CodeGenerationMode::CreateTable) {
		createTableModeWithSqlite();
	} else if (mCodeGenerationMode == CodeGenerationMode::AlterTable) {
		alterTableModeWithSqlite();
	}

	IdList indexes = findIndexes();

	for (Id const &index : indexes) {
		mCodeFile.write("\r\n");
		if (index.editor() == mDbms) {
			mCodeFile.write("CREATE ");

			bool isUnique = mLogicalModelApi.logicalRepoApi().property(index, "isUnique").toBool();
			if (isUnique) {
				mCodeFile.write("UNIQUE ");
			}

			mCodeFile.write("INDEX ");

			bool ifNotExists = mLogicalModelApi.logicalRepoApi().property(index, "if_not_exists").toBool();
			if (ifNotExists) {
				mCodeFile.write("IF NOT EXISTS ");
			}

			QByteArray name = mLogicalModelApi.logicalRepoApi().property(index, "indexName").toByteArray();
			mCodeFile.write(name);

			mCodeFile.write(" ON ");

			Id tableId = Id::loadFromString(mLogicalModelApi.logicalRepoApi().property(index, "tableName").toString());
			QByteArray tableName = getProperty(tableId, "tableName").toByteArray();
			mCodeFile.write(tableName);

			QByteArray columns = mLogicalModelApi.logicalRepoApi().property(index, "columnNames").toByteArray();
			mCodeFile.write(" (");
			mCodeFile.write(columns);
			mCodeFile.write(") ");
		}
	}
}

void DatabasesGenerator::generateWithMicrosoftAccess()
{
	if (mCodeGenerationMode == CodeGenerationMode::CreateTable) {
		createTableModeWithMicrosoftAccess();
	} else if (mCodeGenerationMode == CodeGenerationMode::AlterTable) {
		alterTableModeWithMicrosoftAccess();
	}
}

void DatabasesGenerator::generateWithPostgreSql()
{
	if (mCodeGenerationMode == CodeGenerationMode::CreateTable) {
		createTableModeWithPostgreSql();
	} else if (mCodeGenerationMode == CodeGenerationMode::AlterTable) {
		alterTableModeWithPostgreSql();
	}

	IdList indexes = findIndexes();

	for (Id const &index : indexes) {
		mCodeFile.write("\r\n");
		if (index.editor() == mDbms) {
			mCodeFile.write("CREATE ");

			bool isUnique = mLogicalModelApi.logicalRepoApi().property(index, "isUnique").toBool();
			if (isUnique) {
				mCodeFile.write("UNIQUE ");
			}

			mCodeFile.write("INDEX ");

			bool concurrently = mLogicalModelApi.logicalRepoApi().property(index, "concurrently").toBool();
			if (concurrently) {
				mCodeFile.write("CONCURRENTLY ");
			}

			QByteArray name = mLogicalModelApi.logicalRepoApi().property(index, "indexName").toByteArray();
			mCodeFile.write(name);

			mCodeFile.write(" ON ");

			Id tableId = Id::loadFromString(mLogicalModelApi.logicalRepoApi().property(index, "tableName").toString());
			QByteArray tableName = getProperty(tableId, "tableName").toByteArray();
			mCodeFile.write(tableName);

			mCodeFile.write("USING ");
			QByteArray method = getProperty(index, "columnNames").toByteArray();
			mCodeFile.write(method);

			QByteArray columns = mLogicalModelApi.logicalRepoApi().property(index, "columnNames").toByteArray();
			mCodeFile.write(" (");
			mCodeFile.write(columns);
			mCodeFile.write(") ");
		}
	}
}

void DatabasesGenerator::createTableModeWithSqlServer2008()
{
	if (mPreferencesPage->databaseCreationScriptIsRequired()) {
		QVariant databaseName = QVariant(mPreferencesPage->databaseName());
		mCodeFile.write("CREATE DATABASE ");
		mCodeFile.write(databaseName.toByteArray());
		mCodeFile.write(";");
		mCodeFile.write("\r\n");
	}

	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("CREATE TABLE ");

		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			} else if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}
		}

		QByteArray keyGroups = getProperty(tableId, "key_groups").toByteArray();
		if (!keyGroups.isEmpty()) {
			mCodeFile.write(",\r\n");
			mCodeFile.write(keyGroups);
		}
		mCodeFile.write("\r\n);\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithMySql5()
{
	if (mPreferencesPage->databaseCreationScriptIsRequired()) {
		QVariant databaseName = QVariant(mPreferencesPage->databaseName());
		mCodeFile.write("CREATE DATABASE ");
		mCodeFile.write(databaseName.toByteArray());
		mCodeFile.write(";");
		mCodeFile.write("\r\n");
	}

	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("CREATE ");

		if (getProperty(tableId, "temporary").toBool()) {
			mCodeFile.write("TEMPORARY ");
		}

		mCodeFile.write("TABLE ");

		if (getProperty(tableId, "if_not_exists").toBool()) {
			mCodeFile.write("IF NOT EXISTS ");
		}

		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			} else if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
			if (!defaultValue.isEmpty()) {
				mCodeFile.write(" DEFAULT '" + defaultValue + "'");
			}

			if (getProperty(rowId, "auto_increment").toBool()) {
				mCodeFile.write(" AUTO_INCREMENT");
			}
		}

		QByteArray keyGroups = getProperty(tableId, "key_groups").toByteArray();
		if (!keyGroups.isEmpty()) {
			mCodeFile.write(",\r\n");
			mCodeFile.write(keyGroups);
		}
		mCodeFile.write("\r\n) ");

		QByteArray tableType = getProperty(tableId, "type").toByteArray();
		if (!tableType.isEmpty()) {
			mCodeFile.write(" TYPE " + tableType);
		}

		QByteArray autoIncrementValue = getProperty(tableId, "auto_increment").toByteArray();
		if (!autoIncrementValue.isEmpty()) {
			mCodeFile.write(" AUTO_INCREMENT " + autoIncrementValue);
		}

		QByteArray avgColumnLength = getProperty(tableId, "avg_row_length").toByteArray();
		if (!avgColumnLength.isEmpty()) {
			mCodeFile.write(" AVG_ROW_LENGTH " + avgColumnLength);
		}

		if (getProperty(tableId, "check_sum").toBool()) {
			mCodeFile.write(" CHECKSUM");
		}

		if (!comment.isEmpty()) {
			mCodeFile.write(" COMMENT " + comment);
		}

		QByteArray maxColumns = getProperty(tableId, "max_rows").toByteArray();
		if (!maxColumns.isEmpty()) {
			mCodeFile.write(" MAX_ROWS " + maxColumns);
		}

		QByteArray minColumns = getProperty(tableId, "min_rows").toByteArray();
		if (!minColumns.isEmpty()) {
			mCodeFile.write(" MIN_ROWS " + minColumns);
		}

		if (getProperty(tableId, "pack_keys").toBool()) {
			mCodeFile.write(" PACK_KEYS");
		}

		if (getProperty(tableId, "delay_key_write").toBool()) {
			mCodeFile.write(" DELAY_KEY_WRITE");
		}

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write("\r\n\r\n");
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithSqlite()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("CREATE ");

		if (getProperty(tableId, "temporary").toBool()) {
			mCodeFile.write("TEMPORARY ");
		} else if (getProperty(tableId, "temp").toBool()) {
			mCodeFile.write("TEMP ");
		}

		mCodeFile.write("TABLE ");

		if (getProperty(tableId, "if_not_exists").toBool()) {
			mCodeFile.write("IF NOT EXISTS ");
		}

		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			} else if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
			if (!defaultValue.isEmpty()) {
				mCodeFile.write(" DEFAULT '" + defaultValue + "'");
			}

			if (getProperty(rowId, "auto_increment").toBool()) {
				mCodeFile.write(" AUTO_INCREMENT");
			}
		}

		QByteArray keyGroups = getProperty(tableId, "key_groups").toByteArray();
		if (!keyGroups.isEmpty()) {
			mCodeFile.write(",\r\n");
			mCodeFile.write(keyGroups);
		}
		mCodeFile.write("\r\n) ");

		if (getProperty(tableId, "without_rowid").toBool()) {
			mCodeFile.write(" WITHOUT ROWID");
		}

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write("\r\n\r\n");
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithMicrosoftAccess()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("CREATE ");

		if (getProperty(tableId, "temporary").toBool()) {
			mCodeFile.write("TEMPORARY ");
		}

		mCodeFile.write("TABLE ");

		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			if (getProperty(rowId, "with_compression").toBool()) {
				mCodeFile.write(" WITH COMPRESSION");
			} else if (getProperty(rowId, "with_comp").toBool()) {
				mCodeFile.write(" WITH COMP");
			}
		}

		QByteArray keyGroups = getProperty(tableId, "key_groups").toByteArray();
		if (!keyGroups.isEmpty()) {
			mCodeFile.write(",\r\n");
			mCodeFile.write(keyGroups);
		}
		mCodeFile.write("\r\n);\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::createTableModeWithPostgreSql()
{
	if (mPreferencesPage->databaseCreationScriptIsRequired()) {
		QVariant databaseName = QVariant(mPreferencesPage->databaseName());
		mCodeFile.write("CREATE DATABASE ");
		mCodeFile.write(databaseName.toByteArray());
		mCodeFile.write(";");
		mCodeFile.write("\r\n");
	}

	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("CREATE ");

		if (getProperty(tableId, "global").toBool()) {
			mCodeFile.write("GLOBAL ");
		} else if (getProperty(tableId, "local").toBool()) {
			mCodeFile.write("LOCAL ");
		}

		if (getProperty(tableId, "temporary").toBool()) {
			mCodeFile.write("TEMPORARY ");
		} else if (getProperty(tableId, "temp").toBool()) {
			mCodeFile.write("TEMP ");
		}

		if (getProperty(tableId, "unlogged").toBool()) {
			mCodeFile.write("UNLOGGED ");
		}

		mCodeFile.write("TABLE ");

		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			}

			QByteArray checkExpression = getProperty(rowId, "check").toByteArray();
			if (!checkExpression.isEmpty()) {
				mCodeFile.write(" CHECK");
				mCodeFile.write(checkExpression);
				mCodeFile.write(" ");
			}

			QByteArray defaultValue = getProperty(rowId, "default").toByteArray();
			if (!defaultValue.isEmpty()) {
				mCodeFile.write(" DEFAULT");
				mCodeFile.write(defaultValue);
				mCodeFile.write(" ");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool())
				mCodeFile.write(" PRIMARY KEY");

			if (getProperty(rowId, "unique").toBool())
				mCodeFile.write(" UNIQUE");
		}

		QByteArray keyGroups = getProperty(tableId, "key_groups").toByteArray();
		if (!keyGroups.isEmpty()) {
			mCodeFile.write(",\r\n");
			mCodeFile.write(keyGroups);
		}
		mCodeFile.write("\r\n) ");

		QByteArray inherits = getProperty(tableId, "inherits").toByteArray();
		if (!inherits.isEmpty()) {
			mCodeFile.write(" INHERITS ");
			mCodeFile.write(inherits);
			mCodeFile.write(" ");
		}

		QByteArray with = getProperty(tableId, "with").toByteArray();
		if (!inherits.isEmpty()) {
			mCodeFile.write(" WITH ");
			mCodeFile.write(with);
			mCodeFile.write(" ");
		} else if (getProperty(tableId, "with_oids").toBool()) {
			mCodeFile.write(" WITH OIDS ");
		} else if (getProperty(tableId, "without_oids").toBool()) {
			mCodeFile.write(" WITHOUT OIDS ");
		}

		QByteArray onCommit = getProperty(tableId, "on_commit").toByteArray();
		if (!inherits.isEmpty()) {
			mCodeFile.write(" ON COMMIT ");
			mCodeFile.write(onCommit);
			mCodeFile.write(" ");
		}

		QByteArray tablespace = getProperty(tableId, "tablespace").toByteArray();
		if (!inherits.isEmpty()) {
			mCodeFile.write(" TABLESPACE ");
			mCodeFile.write(tablespace);
			mCodeFile.write(" ");
		}

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write("\r\n\r\n");
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithSqlServer2008()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("ALTER TABLE ");
		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write("ADD COLUMN (");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			} else if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			mCodeFile.write(")");
		}
		mCodeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithMySql5()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("ALTER TABLE ");
		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write("ADD COLUMN (");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			} else if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
			if (!defaultValue.isEmpty()) {
				mCodeFile.write(" DEFAULT '" + defaultValue + "'");
			}

			if (getProperty(rowId, "auto_increment").toBool()) {
				mCodeFile.write(" AUTO_INCREMENT");
			}

			mCodeFile.write(")");
		}
		mCodeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithSqlite()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("ALTER TABLE");
		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write("ADD COLUMN (");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			} else if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			QByteArray defaultValue = getProperty(rowId, "unique").toByteArray();
			if (!defaultValue.isEmpty()) {
				mCodeFile.write(" DEFAULT '" + defaultValue + "'");
			}

			if (getProperty(rowId, "auto_increment").toBool()) {
				mCodeFile.write(" AUTO_INCREMENT");
			}

			mCodeFile.write(")");
		}
		mCodeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithMicrosoftAccess()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("ALTER TABLE ");
		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write("ADD COLUMN (");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}

			if (getProperty(rowId, "with_compression").toBool()) {
				mCodeFile.write(" WITH COMPRESSION");
			} else if (getProperty(rowId, "with_comp").toBool()) {
				mCodeFile.write(" WITH COMP");
			}

			mCodeFile.write(")");
		}
		mCodeFile.write(";\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::alterTableModeWithPostgreSql()
{
	IdList tableNodes = findNodes("Table");
	for (Id const tableId : tableNodes) {
		QByteArray comment = getProperty(tableId, "comment").toByteArray();
		if (!comment.isEmpty()) {
			mCodeFile.write("-- ");
			mCodeFile.write(comment);
			mCodeFile.write("\r\n");
		}

		mCodeFile.write("ALTER TABLE ");
		mCodeFile.write(getProperty(tableId, "tableName").toByteArray());
		mCodeFile.write("\r\n(");
		IdList rowsSet = getChildren(tableId);

		bool first = true;
		for (Id const &rowId : rowsSet) {
			if (!first) {
				mCodeFile.write(",");
			}
			first = false;
			mCodeFile.write("\r\n");
			mCodeFile.write("ADD COLUMN (");
			mCodeFile.write(getProperty(rowId, "columnName").toByteArray());
			mCodeFile.write(" ");
			mCodeFile.write(getProperty(rowId, "DataType").toByteArray());

			if (getProperty(rowId, "notNull").toBool()) {
				mCodeFile.write(" NOT NULL");
			}

			if (getProperty(rowId, "null").toBool()) {
				mCodeFile.write(" NULL");
			}

			QByteArray checkExpression = getProperty(rowId, "check").toByteArray();
			if (!checkExpression.isEmpty()) {
				mCodeFile.write(" CHECK");
				mCodeFile.write(checkExpression);
				mCodeFile.write(" ");
			}

			QByteArray defaultValue = getProperty(rowId, "default").toByteArray();
			if (!defaultValue.isEmpty()) {
				mCodeFile.write(" DEFAULT");
				mCodeFile.write(defaultValue);
				mCodeFile.write(" ");
			}

			if (getProperty(rowId, "isPrimaryKey").toBool()) {
				mCodeFile.write(" PRIMARY KEY");
			}

			if (getProperty(rowId, "unique").toBool()) {
				mCodeFile.write(" UNIQUE");
			}
			mCodeFile.write(")");
		}

		mCodeFile.write("\r\n\r\n");

		QByteArray query = getProperty(tableId, "query").toByteArray();
		if (!query.isEmpty()) {
			mCodeFile.write(query);
			mCodeFile.write("\r\n\r\n");
		}
	}
}

void DatabasesGenerator::changeEditor(QString const &dbmsName)
{
	mDbms = dbmsName;
}

}
}
