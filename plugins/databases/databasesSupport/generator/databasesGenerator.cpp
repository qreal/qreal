#include "databasesGenerator.h"
#include <QtWidgets/QApplication>

namespace qReal {
namespace databasesSupport {

QString const databases = "Databases";

DatabasesGenerator::DatabasesGenerator(PluginConfigurator const configurator, DatabasesPreferencesPage const *preferencesPage)
	: mCurrentDiagram(Id::rootId())
	, mCurrentId(Id::rootId())
	, mCodeFileName("code.txt")
	, mWorkDir("")
	, mLogicalModelApi(configurator.logicalModelApi())
	, mGraphicalModelApi(configurator.graphicalModelApi())
	, mInterpretersInterface(configurator.mainWindowInterpretersInterface())
	, mErrorReporter(configurator.mainWindowInterpretersInterface().errorReporter())
	, mDbms(QString("Sql_server_2008"))
	, mPreferencesPage(preferencesPage)
//	, mDatatypesChecker(new DatatypesChecker("", configurator.mainWindowInterpretersInterface().errorReporter()))
{
	connect(mPreferencesPage, SIGNAL(dbmsChanged(QString)), this, SLOT(changeEditor(QString)));
	//mAppTranslator.load(":/DatabasesGenerator_" + QLocale::system().name());
	//QApplication::installTranslator(&mAppTranslator);

	//mParser = new BlockParser(mErrorReporter);
}

DatabasesGenerator::~DatabasesGenerator()
{
//	delete mDatatypesChecker;
}

void DatabasesGenerator::activeTabChanged(Id const &rootElementId)
{
	/*bool const enabled = rootElementId.diagram() == blockDiagram;
	foreach (ActionInfo const &actionInfo, mActionInfos) {
		if (actionInfo.isAction()) {
			actionInfo.action()->setEnabled(enabled);
		} else {
			actionInfo.menu()->setEnabled(enabled);
		}
	}*/
}

void DatabasesGenerator::clearPhysicalModel()
{
	IdList tableNodes = findNodes("DatabasesPhysicalNode");
	foreach (Id const &id, tableNodes) {
		mLogicalModelApi.removeElement(id);
		//mGraphicalModelApi.removeElement(mGraphicalModelApi.graphicalIdsByLogicalId(id).first());
	}
}

void DatabasesGenerator::setCodeFileName(QString const &name)
{
	mCodeFileName = name;
}

void DatabasesGenerator::setWorkDir(QString const &path)
{
	if (path != "") {
		mWorkDir = path + "/";
	}
}

IdList DatabasesGenerator::findNodes(QString const &name)
{
	IdList const children = mGraphicalModelApi.graphicalRepoApi().children(mCurrentDiagram);
	IdList filteredChildren;
	foreach (Id const &child, children) {
		if (child.element() == name) {
			filteredChildren.append(child);
		}
	}
	return filteredChildren;
	//error(VisualDebugger::missingBeginNode);
	//return Id::rootId();
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
	//Id s = mGraphicalModelApi.graphicalRepoApi().parent(mGraphicalModelApi.graphicalIdsByLogicalId(child).at(0));

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

	foreach (Id const &relationship, relationshipsOut) {
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
	foreach (Id const &relationship, relationshipsIn) {
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

void DatabasesGenerator::error(QString error, bool isCritical)
{
	if (isCritical) {
		mInterpretersInterface.errorReporter()->addCritical(error);
	} else {
		mInterpretersInterface.errorReporter()->addWarning(error, Id::rootId());
	}
	//mCurrentId = Id::rootId();
	//mCurrentDiagram = Id::rootId();
}

Id DatabasesGenerator::getPrimaryKey(Id const &entityId)
{
	IdList attributesSet = getChildren(entityId);
	if(attributesSet.isEmpty()) {
		return Id::rootId();
	}
	Id primaryKey = Id::rootId();
	int primaryKeyCount = 0;
	foreach (Id const &attributeId, attributesSet) {
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

int DatabasesGenerator::getParentList(Id const &childEntity, QList<IdList> set)
{
	int listCounter = 0;
	foreach (IdList const &list, set) {
		if (list.indexOf(childEntity) != -1)
			return listCounter;
		listCounter++;
	}
	return -1;
}

QString DatabasesGenerator::getListTableName(IdList const &list)
{
	QString name = "";
	bool first = true;
	foreach (Id const &id, list) {
		if (!first) {
			name += "And";
		} else {
			first = false;
		}
		name += getProperty(id, "Name").toByteArray();
	}
	return name;
}

bool DatabasesGenerator::checkEntities()
{
	bool result = true;
	IdList entities = findNodes("Entity");
	foreach (Id const &entity, entities) {
		QString name = getProperty(entity, "Name").toString();
		if (name == "") {
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
	foreach (Id const &attribute, attributes) {
		QString name = getProperty(attribute, "Name").toString();
		QString datatype = getProperty(attribute, "DataType").toString();
		Id parent = getParent(attribute);
		QString parentName = getProperty(parent, "Name").toString();
		if (name == "") {
			result = false;
			error(tr("Attribute has no name in entity '") + parentName + "'", true);
		}
		if (datatype == "") {
			result = false;
			error(tr("Attribute has no datatype in entity '") + parentName + "'", true);
		}
/*		if (!mDatatypesChecker->isDatatype(datatype)) {
			result = false;
			error(tr("Invalid datatype in entity '") + parentName + "'", true);
		}*/
	}
	return result;
}

bool DatabasesGenerator::checkRelationships()
{
	bool result = true;
	IdList relationships = findNodes("OneToManyRelationship");
	relationships.append(findNodes("ManyToManyRelationship"));
	relationships.append(findNodes("OneToOneRelationship"));
	foreach (Id const &relationship, relationships) {
		Id out = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		if (out == Id::rootId() || from == Id::rootId()) {
			result = false;
			error(getProperty(relationship, "name").toString() + tr(" relationship with name '") + getProperty(relationship, "Name").toString() + tr("' has invalid ends"), true);
		}
	}
	return result;
}

bool DatabasesGenerator::checkCorrectness()
{
	bool checkOne = checkRelationships();
	bool checkTwo = checkAttributes();
	bool checkThree = checkEntities();
	if (checkOne && checkTwo && checkThree) {
		mErrorReporter->addInformation("Diagram is correct.");
	}
	return checkOne && checkTwo && checkThree;
}



qReal::Id DatabasesGenerator::createElementFromString(QString const &elemName, QPointF coord, Id const &parentLogicalId, bool coordByParent)
{
	Id id = Id::loadFromString(QString("qrm:/" + mDbms + "/DatabasesPhysicalModelMetamodel/" + elemName));
	Id logicalId = mLogicalModelApi.createElement(parentLogicalId, id);
	Id graphicalParentId = Id::rootId();
	if (parentLogicalId != Id::rootId()) {
		graphicalParentId = mGraphicalModelApi.graphicalIdsByLogicalId(parentLogicalId).first();
		if (coordByParent)
			coord = mGraphicalModelApi.position(graphicalParentId);
	}
	mGraphicalModelApi.createElement(graphicalParentId, logicalId, true, elemName, coord);
	return logicalId;
}

qReal::Id DatabasesGenerator::makeRowFromAttribute(Id const &attributeId, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(attributeId);
	Id logicalRowId = createElementFromString("Row", QPointF(), parentId);
	QString rowName = getProperty(attributeId, "Name").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalRowId, rowName, "Name");
	return logicalRowId;
}

qReal::Id DatabasesGenerator::makeTableFromEntity(Id const &entityId, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(entityId).first());
	Id logicalTableId = createElementFromString("Table", coord, parentId);
	QString tableName = getProperty(entityId, "Name").toString();
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "Name");

	IdList attributesSet = getChildren(entityId);
	foreach (Id const &attributeId, attributesSet) {
		makeRowFromAttribute(attributeId, logicalTableId);
	}
	return logicalTableId;
}

qReal::Id DatabasesGenerator::makeTableFromEntitySet(IdList const &set, Id const &parentId)
{
	QPointF coord = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(set.first()).first());
	Id logicalTableId = createElementFromString("Table", coord, parentId);
	QString tableName = getListTableName(set);
	mLogicalModelApi.setPropertyByRoleName(logicalTableId, tableName, "Name");

	foreach(Id const &entityId, set){
		IdList attributes = getChildren(entityId);
		foreach (Id const &attributeId, attributes) {
			makeRowFromAttribute(attributeId, logicalTableId);
		}
	}
	return logicalTableId;
}

qReal::Id DatabasesGenerator::copyOneToManyRelationship(Id const &logicalModelId, Id const &parentId, Id const &from, Id const &to)
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

qReal::Id DatabasesGenerator::copyManyToManyRelationship(Id const &logicalModelId, Id const &parentId, Id const &from, Id const &to)
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

/// Search for alone entities (we make table for each alone entity)
/// Formation of one-to-one sets (we will union entities bounded by one-to-one relationship in one table)
QList<IdList> DatabasesGenerator::processEntities(Id const &logicalDiagramId)
{
	QList<IdList> oneToOneBoundedEntitiesSets;
	mPassedElements.clear();
	IdList entityNodes = findNodes("Entity");
	foreach (Id const &entityId, entityNodes) {
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

bool DatabasesGenerator::processOneToManyRelationships(QList<IdList> oneToOneBoundedEntitiesSets, IdList setTables, int **match, Id const &logicalDiagramId)
{
	IdList oneToManyRelationships = findNodes("OneToManyRelationship");
	foreach (Id const &relationship, oneToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneBoundedEntitiesSets);
		int fromSet = getParentList(from, oneToOneBoundedEntitiesSets);

		if (match[toSet][fromSet] != 0) {
			error("Too many relationships from "+ getProperty(from, "Name").toString() + "to " + getProperty(to, "Name").toString(), true);
			return false;
		}

		QString columnNameForRelationship = getProperty(relationship, "ColumnName").toString();
		if (columnNameForRelationship.isEmpty()) {
			mErrorReporter->addInformation(tr("Column name for one-to-many relationship with name '") + getProperty(relationship, "Name").toString()+ tr("' is`n specified. Column name was generated automatically."));
			columnNameForRelationship = getPrimaryKeyNameOfSet(oneToOneBoundedEntitiesSets.at(toSet));
		}
		match[fromSet][toSet] = 1;
		match[toSet][fromSet] = -1;

		//add bounding attribute
		Id logicalRowId = createElementFromString("Row", QPointF(), setTables.at(fromSet), true);
		QString rowName = getProperty(relationship, "ColumnName").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalRowId, rowName, "Name");
		// copy relationship
		copyOneToManyRelationship(relationship, logicalDiagramId, setTables.at(fromSet), setTables.at(toSet));
	}
	return true;
}

bool DatabasesGenerator::processManyToManyRelationships(QList<IdList> oneToOneBoundedEntitiesSets, IdList setTables, int **match, Id const &logicalDiagramId)
{
	IdList manyToManyRelationships = findNodes("ManyToManyRelationship");
	foreach (Id const &relationship, manyToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneBoundedEntitiesSets);
		int fromSet = getParentList(from, oneToOneBoundedEntitiesSets);

		if (match[toSet][fromSet] != 0) {
			error(tr("Too many relationships from ") + getProperty(from, "Name").toString() + tr(" to ") + getProperty(to, "Name").toString(), true);
			return false;
		}

		QString relationshipTableName = getProperty(relationship, "TableName").toString();
		if (relationshipTableName.isEmpty()) {
			mErrorReporter->addInformation(tr("Table name for many-to-many relationship with name '") + getProperty(relationship, "Name").toString() +  tr("' is`t specified. Table name was generated automatically"), Id::rootId());
			relationshipTableName = (getListTableName(oneToOneBoundedEntitiesSets.at(fromSet)) + "_" + getListTableName(oneToOneBoundedEntitiesSets.at(toSet))).toUtf8();
		}
		match[toSet][fromSet] = 2;
		match[toSet][fromSet] = 2;

		// Creating
		Id logicalTableId = createElementFromString("Table", QPointF(), logicalDiagramId);
		mLogicalModelApi.setPropertyByRoleName(logicalTableId, relationshipTableName, "Name");

		//copy relationship
		copyManyToManyRelationship(relationship, logicalDiagramId, setTables.at(fromSet), logicalTableId);
		copyManyToManyRelationship(relationship, logicalDiagramId, logicalTableId, setTables.at(toSet));

		//add bounding attribute
		Id logicalRowIdOne = createElementFromString("Row", QPointF(), logicalTableId, true);
		QString rowName = "keyOf" + getProperty(setTables.at(fromSet), "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalRowIdOne, rowName, "Name");

		//add bounding attribute 2
		Id logicalRowIdTwo = createElementFromString("Row", QPointF(), logicalTableId, true);
		QString rowName2 = "keyOf" + getProperty(setTables.at(toSet), "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(logicalRowIdTwo, rowName2, "Name");
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
	foreach(IdList const &set, oneToOneBoundedEntitiesSets) {
		Id logicalTableId = makeTableFromEntitySet(set, logicalDiagramId);
		setTables.append(logicalTableId);
	}

	// match[i][k] =
	// 0 - no relationships
	// 1 - one-to-many relationship (i - many, k - one)
	// -1 - one-to-many relationship (i - one, k - many)
	// 2 - many-to-many relationship
	// 3 - one-to-one relationship
	int oneToOneSetsSize = oneToOneBoundedEntitiesSets.size();
	int **match = new int*[oneToOneSetsSize];
	for (int i = 0; i < oneToOneSetsSize; i++) {
		match[i] = new int[oneToOneSetsSize];
		for (int k = 0; k < oneToOneSetsSize; k++)
			match[i][k] = 0;
		match[i][i] = 3;
	}

	if (!processOneToManyRelationships(oneToOneBoundedEntitiesSets, setTables, match, logicalDiagramId))
		return;
	if (!processManyToManyRelationships(oneToOneBoundedEntitiesSets, setTables, match, logicalDiagramId))
		return;

	for (int i = 0; i < oneToOneSetsSize; i++) {
		delete[] match[i];
	}
	delete[] match;
	mErrorReporter->addInformation(tr("Physical model was generated successfully"));
}

void DatabasesGenerator::generateSQLCode()
{
	if (mDbms = "Sql_server_2008")
		generateWithSqlServer2008();
}

void DatabasesGenerator::generateWithSqlServer2008()
{
	mErrorReporter->clear();

	codeFile.setFileName(mWorkDir + mCodeFileName);
	if (!codeFile.open(QIODevice::WriteOnly))
		return;

	IdList tableNodes = findNodes("Table");
	foreach (Id const tableId, tableNodes) {
			codeFile.write("CREATE TABLE ");
			codeFile.write(getProperty(tableId, "Name").toByteArray());
			codeFile.write("\r\n(");
			IdList rowsSet = getChildren(tableId);

			bool first = true;
			foreach (Id const &rowId, rowsSet) {
				if (!first) {
					codeFile.write(",");
				}
				first = false;
				codeFile.write("\r\n");
				codeFile.write(getProperty(rowId, "Name").toByteArray());
				codeFile.write(" ");
				codeFile.write(getProperty(rowId, "DataType").toByteArray());
			}
			codeFile.write("\r\n);\r\n\r\n");
		}
	codeFile.close();
	mErrorReporter->addInformation(tr("Code was generated successfully"));
}

void DatabasesGenerator::changeEditor(QString const &dbmsName)
{
	if (dbmsName == "Microsoft SQL Server 2008")
		mDbms = "Sql_server_2008";
}

}
}
