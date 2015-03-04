#include "databasesGenerator.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace databasesSupport;

QString const databases = "Databases";

DatabasesGenerator::DatabasesGenerator(const PluginConfigurator configurator)
	: mCurrentDiagram(Id::rootId())
	, mCurrentId(Id::rootId())
	, mCodeFileName("code.txt")
	, mWorkDir("")
	, mLogicalModelApi(configurator.logicalModelApi())
	, mGraphicalModelApi(configurator.graphicalModelApi())
	, mInterpretersInterface(configurator.mainWindowInterpretersInterface())
	, mErrorReporter(configurator.mainWindowInterpretersInterface().errorReporter())
	, mDatatypesChecker(new DatatypesChecker("", configurator.mainWindowInterpretersInterface().errorReporter()))
{
	//mAppTranslator.load(":/DatabasesGenerator_" + QLocale::system().name());
	//QApplication::installTranslator(&mAppTranslator);

	//mParser = new BlockParser(mErrorReporter);
}

DatabasesGenerator::~DatabasesGenerator()
{
	delete mDatatypesChecker;
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
		error("Invalid number of primary key in entity with name '" + getProperty(entityId, "Name").toString() +  "': " + QString::number(primaryKeyCount), true);
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
		if (!mDatatypesChecker->isDatatype(datatype)) {
			result = false;
			error(tr("Invalid datatype in entity '") + parentName + "'", true);
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

void DatabasesGenerator::checkCorrectness()
{
	bool checkOne = checkRelationships();
	bool checkTwo = checkAttributes();
	bool checkThree = checkEntities();
	if (checkOne && checkTwo && checkThree) {
		mErrorReporter->addInformation("Diagram is correct.");
	}
}

void DatabasesGenerator::generateSQL()
{
	mErrorReporter->clear();
	mPassedElements.clear();

	bool checkRel = checkRelationships();
	bool checkAtt = checkAttributes();
	bool checkEnt = checkEntities();
	if (!(checkRel && checkAtt && checkEnt)) {
		return;
	}

	codeFile.setFileName(mWorkDir + mCodeFileName);
	int y = codeFile.open(QIODevice::WriteOnly);

	IdList entityNodes = findNodes("Entity");

	QList<IdList> oneToOneAllTablesSet;
	oneToOneAllTablesSet.clear();

/// Search for alone entities (we make table for each alone entity)
/// Formation of one-to-one sets (we union entities bounded by one-to-one relationship in one table)
	foreach (Id const &entityId, entityNodes) {
		IdList relationships = mLogicalModelApi.logicalRepoApi().outgoingLinks(entityId);
		relationships.append(mLogicalModelApi.logicalRepoApi().incomingLinks(entityId));

		if (relationships.isEmpty()) {
			mPassedElements.append(entityId);
			codeFile.write("CREATE TABLE ");
			codeFile.write(getProperty(entityId, "Name").toByteArray());
			codeFile.write("\r\n(");
			IdList attributesSet = getChildren(entityId);
			if (attributesSet.isEmpty()) {
				error("Entity without attributes with name '" + getProperty(entityId, "Name").toString() + "'", false);
			}

			bool first = true;
			foreach (Id const &attribute, attributesSet) {
				if (!first) {
					codeFile.write(",");
				}
				first = false;
				codeFile.write("\r\n");
				codeFile.write(getProperty(attribute, "Name").toByteArray());
				codeFile.write(" ");
				codeFile.write(getProperty(attribute, "DataType").toByteArray());
			}
			codeFile.write("\r\n);\r\n\r\n");
		} else {
			if (mPassedElements.indexOf(entityId) == -1) {
				mPassedElements.append(entityId);
				// List for set of tables bounded with one-to-one relationship
				IdList oneToOneTableSet;
				oneToOneTableSet.clear();
				oneToOneTableSet = getBoundedWithOneToOneRealationship(entityId);
				oneToOneAllTablesSet.append(oneToOneTableSet);
			}
		}
	}

	QString* extraAttributes = new QString[oneToOneAllTablesSet.size()];
	for(int i = 0; i < oneToOneAllTablesSet.size(); i++) {
		extraAttributes[i] = "";
	}

	// match[i][k] =
	// 0 - no relationships
	// 1 - one-to-many relationship (i - many, k - one)
	// -1 - one-to-many relationship (i - one, k - many)
	// 2 - many-to-many relationship
	// 3 - one-to-one relationship
	int oneToOneAllTablesSetSize = oneToOneAllTablesSet.size();
	int** match = new int*[oneToOneAllTablesSetSize];
	for (int i = 0; i < oneToOneAllTablesSetSize; i++) {
		match[i] = new int[oneToOneAllTablesSetSize];
		for (int k = 0; k < oneToOneAllTablesSetSize; k++)
			match[i][k] = 0;
		match[i][i] = 3;
	}

	IdList oneToManyRelationships = findNodes("OneToManyRelationship");
	foreach (Id const &relationship, oneToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneAllTablesSet);
		int fromSet = getParentList(from, oneToOneAllTablesSet);

		if (match[toSet][fromSet] != 0) {
			error("Too many relationships from "+ getProperty(from, "Name").toString() + "to " + getProperty(to, "Name").toString(), true);
			return;
		}

		Id toPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(toSet));
		if (toPrimaryKey == Id::rootId()) {
			return;
		}

		QString columnNameForRelationship = getProperty(relationship, "ColumnName").toString();
		if (columnNameForRelationship.isEmpty()) {
			mErrorReporter->addInformation(tr("Column name for one-to-many relationship with name '") + getProperty(relationship, "Name").toString()+ tr("' is`n specified. Column name was generated automatically."));
			columnNameForRelationship = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(toSet));
		}
		extraAttributes[fromSet] += (",\r\n" + columnNameForRelationship + " " + getProperty(toPrimaryKey, "DataType").toString());

		match[fromSet][toSet] = 1;
		match[toSet][fromSet] = -1;
	}

	IdList manyToManyRelationships = findNodes("ManyToManyRelationship");
	foreach (Id const &relationship, manyToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneAllTablesSet);
		int fromSet = getParentList(from, oneToOneAllTablesSet);

		if (match[toSet][fromSet] != 0) {
			error(tr("Too many relationships from ") + getProperty(from, "Name").toString() + tr(" to ") + getProperty(to, "Name").toString(), true);
			return;
		}

		Id toPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(toSet));
		if (toPrimaryKey == Id::rootId()) {
			return;
		}
		QString toPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(toSet));
		Id fromPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(fromSet));
		QString fromPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(fromSet));
		if (fromPrimaryKey == Id::rootId()) {
			return;
		}

		codeFile.write("CREATE TABLE ");
		QString relationshipTableName = getProperty(relationship, "TableName").toString();
		if (relationshipTableName.isEmpty()) {
			mErrorReporter->addInformation(tr("Table name for many-to-many relationship with name '") + getProperty(relationship, "Name").toString() +  tr("' is`t specified. Table name was generated automatically"), Id::rootId());
			relationshipTableName = (getListTableName(oneToOneAllTablesSet.at(fromSet)) + "_" + getListTableName(oneToOneAllTablesSet.at(toSet))).toUtf8();
		}
		codeFile.write(relationshipTableName.toUtf8());
		codeFile.write("\r\n(");
		codeFile.write("\r\n");

		codeFile.write((toPrimaryKeyName + " " + getProperty(toPrimaryKey, "DataType").toByteArray()).toUtf8());
		codeFile.write(",\r\n");
		codeFile.write((fromPrimaryKeyName + " " + getProperty(fromPrimaryKey, "DataType").toByteArray()).toUtf8());
		codeFile.write("\r\n);\r\n\r\n");

		match[toSet][fromSet] = 2;
		match[toSet][fromSet] = 2;
	}

	int i = 0;
	foreach (IdList const &list, oneToOneAllTablesSet) {
		codeFile.write("CREATE TABLE ");
		codeFile.write(getListTableName(list).toUtf8());
		codeFile.write("\r\n(");

		IdList attributesSet;
		foreach (Id const &id, list) {
			attributesSet.append(getChildren(id));
		}

		bool first = true;
		foreach (Id const &attribute, attributesSet) {
			if (!first) {
				codeFile.write(",");
			}
			first = false;
			codeFile.write("\r\n");
			codeFile.write(getProperty(attribute, "Name").toByteArray());
			codeFile.write(" ");
			codeFile.write(getProperty(attribute, "DataType").toByteArray());
			if (getProperty(attribute, "isPrimaryKey").toBool()) {
				codeFile.write(" PRIMARY KEY");
			} else {
				if (getProperty(attribute, "isUnique").toBool()) {
					codeFile.write(" UNIQUE");
				}
				if (getProperty(attribute, "isNotNull").toBool()) {
					codeFile.write(" NOT NULL");
				}
			}
		}
		codeFile.write(extraAttributes[i].toUtf8());
		i++;
		codeFile.write("\r\n);\r\n\r\n");
	}
	delete[] extraAttributes;
	for (int i = 0; i < oneToOneAllTablesSetSize; i++) {
		delete[] match[i];
	}
	delete[] match;
	codeFile.close();
	mErrorReporter->addInformation(tr("Code was generated successfully"));
}

/*// logical id s
Id createCopy(Id patternId, QString idLoader, Id parentId, bool patternIdIsLogical, bool parentIdIsLogical, bool resultIdIsLogical)
{
	Id id = Id::loadFromString(idLoader);

	Id createdLogicalId = mLogicalModelApi.createElement(parentId, id);
	QPointF coordinates;
	if (patternIdIsLogical)
		coordinates = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(patternId).first());
	else
		coordinates = mGraphicalModelApi.position(patternId);
	Id createdGraphicalTableId = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalTableId, true, QString("Table"), tableCoordinates);


}*/

void DatabasesGenerator::generatePhysicalModel()
{
	mErrorReporter->clear();
	mPassedElements.clear();

	bool checkRel = checkRelationships();
	bool checkAtt = checkAttributes();
	bool checkEnt = checkEntities();
	if (!(checkRel && checkAtt && checkEnt)) {
		return;
	}

	Id diagramId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/DatabasesPhysicalNode"));
	Id createdLogicalDiagramId = mLogicalModelApi.createElement(Id::rootId(), diagramId);
	mGraphicalModelApi.createElement(Id::rootId(), createdLogicalDiagramId, true, QString("Databases Physical Model"), QPointF(23.0, 23.0));
	Id createdGraphicalDiagramId = mGraphicalModelApi.graphicalIdsByLogicalId(createdLogicalDiagramId).first();

	QList<IdList> oneToOneBoundedEntitiesSets;
	oneToOneBoundedEntitiesSets;

/// Search for alone entities (we make table for each alone entity)
/// Formation of one-to-one sets (we will union entities bounded by one-to-one relationship in one table)
	IdList entityNodes = findNodes("Entity");
	foreach (Id const &entityId, entityNodes) {
		IdList relationships = mLogicalModelApi.logicalRepoApi().outgoingLinks(entityId);
		relationships.append(mLogicalModelApi.logicalRepoApi().incomingLinks(entityId));
		// If alone entity
		if (relationships.isEmpty()) {
			mPassedElements.append(entityId);

			// Creating
			Id tableId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Table"));
			Id createdLogicalTableId = mLogicalModelApi.createElement(Id::rootId(), tableId);
			QPointF tableCoordinates = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(entityId).first());
			Id createdGraphicalTableId = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalTableId, true, QString("Table"), tableCoordinates);

			// Copying properties and childrens
			QString tableName = getProperty(entityId, "Name").toString();
			mLogicalModelApi.setPropertyByRoleName(createdLogicalTableId, tableName, "Name");

			IdList attributesSet = getChildren(entityId);
			foreach (Id const &attributeId, attributesSet) {
				Id rowId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Row"));
				Id createdLogicalRowId = mLogicalModelApi.createElement(Id::rootId(), rowId);
				QPointF rowCoordinates = mGraphicalModelApi.position(attributeId);
				mGraphicalModelApi.createElement(createdGraphicalTableId, createdLogicalRowId, true, QString("Row"), rowCoordinates);

				QString rowName = getProperty(attributeId, "Name").toString();
				mLogicalModelApi.setPropertyByRoleName(createdLogicalRowId, rowName, "Name");
			}
		} else {
		// if not alone entity
		// make set
			if (mPassedElements.indexOf(entityId) == -1) {
				mPassedElements.append(entityId);
				// List for set of tables bounded with one-to-one relationship
				IdList oneToOneBoundedEntitySet;
				oneToOneBoundedEntitySet.clear();
				oneToOneBoundedEntitySet = getBoundedWithOneToOneRealationship(entityId);
				oneToOneBoundedEntitiesSets.append(oneToOneBoundedEntitySet);
			}
		}
	}

	// match[i][k] =
	// 0 - no relationships
	// 1 - one-to-many relationship (i - many, k - one)
	// -1 - one-to-many relationship (i - one, k - many)
	// 2 - many-to-many relationship
	// 3 - one-to-one relationship
	int oneToOneSetsSize = oneToOneBoundedEntitiesSets.size();
	int** match = new int*[oneToOneSetsSize];
	for (int i = 0; i < oneToOneSetsSize; i++) {
		match[i] = new int[oneToOneSetsSize];
		for (int k = 0; k < oneToOneSetsSize; k++)
			match[i][k] = 0;
		match[i][i] = 3;
	}

	IdList setTables;
	foreach(IdList const &set, oneToOneBoundedEntitiesSets) {
		// Creating
		Id tableId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Table"));
		Id createdLogicalTableId = mLogicalModelApi.createElement(Id::rootId(), tableId);
		setTables.append(createdLogicalTableId);
		QPointF tableCoordinates = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(set.first()).first());
		Id createdGraphicalTableId = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalTableId, true, QString("Table"), tableCoordinates);

		// Copying properties and childrens
		QString tableName = getListTableName(set);
		mLogicalModelApi.setPropertyByRoleName(createdLogicalTableId, tableName, "Name");

		IdList attributesSet;
		foreach(Id const &entityId, set){
			IdList attributes = getChildren(entityId);
			attributesSet.append(attributes);
		}

		foreach (Id const &attributeId, attributesSet) {
			Id rowId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Row"));
			Id createdLogicalRowId = mLogicalModelApi.createElement(Id::rootId(), rowId);
			QPointF rowCoordinates = mGraphicalModelApi.position(attributeId);
			mGraphicalModelApi.createElement(createdGraphicalTableId, createdLogicalRowId, true, QString("Row"), rowCoordinates);

			QString rowName = getProperty(attributeId, "Name").toString();
			mLogicalModelApi.setPropertyByRoleName(createdLogicalRowId, rowName, "Name");
			}
	}

	IdList oneToManyRelationships = findNodes("OneToManyRelationship");
	foreach (Id const &relationship, oneToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneBoundedEntitiesSets);
		int fromSet = getParentList(from, oneToOneBoundedEntitiesSets);

		if (match[toSet][fromSet] != 0) {
			error("Too many relationships from "+ getProperty(from, "Name").toString() + "to " + getProperty(to, "Name").toString(), true);
			return;
		}

		/*Id toPrimaryKey = getPrimaryKeyOfSet(oneToOneBoundedEntitiesSets.at(toSet));
		if (toPrimaryKey == Id::rootId()) {
			return;
		}
*/
		QString columnNameForRelationship = getProperty(relationship, "ColumnName").toString();
		if (columnNameForRelationship.isEmpty()) {
			mErrorReporter->addInformation(tr("Column name for one-to-many relationship with name '") + getProperty(relationship, "Name").toString()+ tr("' is`n specified. Column name was generated automatically."));
			columnNameForRelationship = getPrimaryKeyNameOfSet(oneToOneBoundedEntitiesSets.at(toSet));
		}
		match[fromSet][toSet] = 1;
		match[toSet][fromSet] = -1;

		//add bounding attribute
		Id rowId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Row"));
		Id createdLogicalRowId = mLogicalModelApi.createElement(Id::rootId(), rowId);
		QPointF rowCoordinates = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(fromSet)).first());
		mGraphicalModelApi.createElement(mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(fromSet)).first(), createdLogicalRowId, true, QString("Row"), rowCoordinates);

		QString rowName = getProperty(relationship, "ColumnName").toString();
		mLogicalModelApi.setPropertyByRoleName(createdLogicalRowId, rowName, "Name");

		//copy relationship
		Id newRelationshipId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/PhysicalOneToManyRelationship"));
		Id createdLogicalRelationshipId = mLogicalModelApi.createElement(Id::rootId(), newRelationshipId);
		QPointF relationshipCoordinates = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(relationship).first());
		Id createdGraphicalRelationshipId = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalRelationshipId, true, QString("One-to-many"), relationshipCoordinates);

		QString relationshipName = getProperty(relationship, "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(createdLogicalRelationshipId, relationshipName, "Name");

		mLogicalModelApi.setFrom(createdLogicalRelationshipId, setTables.at(fromSet));
		mLogicalModelApi.setTo(createdLogicalRelationshipId, setTables.at(toSet));

		mGraphicalModelApi.setFrom(createdGraphicalRelationshipId, mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(fromSet)).first());
		mGraphicalModelApi.setTo(createdGraphicalRelationshipId,  mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(toSet)).first());

	}

	IdList manyToManyRelationships = findNodes("ManyToManyRelationship");
	foreach (Id const &relationship, manyToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneBoundedEntitiesSets);
		int fromSet = getParentList(from, oneToOneBoundedEntitiesSets);

		if (match[toSet][fromSet] != 0) {
			error(tr("Too many relationships from ") + getProperty(from, "Name").toString() + tr(" to ") + getProperty(to, "Name").toString(), true);
			return;
		}

		/*Id toPrimaryKey = getPrimaryKeyOfSet(oneToOneBoundedEntitiesSets.at(toSet));
		if (toPrimaryKey == Id::rootId()) {
			return;
		}
		//QString toPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(toSet));
		Id fromPrimaryKey = getPrimaryKeyOfSet(oneToOneBoundedEntitiesSets.at(fromSet));
		//QString fromPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(fromSet));
		if (fromPrimaryKey == Id::rootId()) {
			return;
		}*/

		QString relationshipTableName = getProperty(relationship, "TableName").toString();
		if (relationshipTableName.isEmpty()) {
			mErrorReporter->addInformation(tr("Table name for many-to-many relationship with name '") + getProperty(relationship, "Name").toString() +  tr("' is`t specified. Table name was generated automatically"), Id::rootId());
			relationshipTableName = (getListTableName(oneToOneBoundedEntitiesSets.at(fromSet)) + "_" + getListTableName(oneToOneBoundedEntitiesSets.at(toSet))).toUtf8();
		}
		match[toSet][fromSet] = 2;
		match[toSet][fromSet] = 2;

		// Creating
		Id tableId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Table"));
		Id createdLogicalTableId = mLogicalModelApi.createElement(Id::rootId(), tableId);
		Id createdGraphicalTableId = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalTableId, true, QString("Table"), QPointF());
		mLogicalModelApi.setPropertyByRoleName(createdLogicalTableId, relationshipTableName, "Name");

		//copy relationship 1
		Id newRelationshipId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/PhysicalManyToManyRelationship"));
		Id createdLogicalRelationshipId = mLogicalModelApi.createElement(Id::rootId(), newRelationshipId);
		//QPointF relationshipCoordinates = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(relationship).first());
		Id createdGraphicalRelationshipId = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalRelationshipId, true, QString("Many-to-many"), QPointF());

		QString relationshipName = getProperty(relationship, "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(createdLogicalRelationshipId, relationshipName, "Name");

		mLogicalModelApi.setFrom(newRelationshipId, setTables.at(fromSet));
		mLogicalModelApi.setTo(newRelationshipId, createdLogicalTableId);
		mGraphicalModelApi.setTo(createdGraphicalRelationshipId, createdGraphicalTableId);
		mGraphicalModelApi.setFrom(createdGraphicalRelationshipId, mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(fromSet)).first());
		//mLogicalModelApi.setTo(newRelationshipId, setTables.at(toSet));

		//copy relationship 1
		Id newRelationshipId2 = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/PhysicalManyToManyRelationship"));
		Id createdLogicalRelationshipId2 = mLogicalModelApi.createElement(Id::rootId(), newRelationshipId2);
		//QPointF relationshipCoordinates2 = mGraphicalModelApi.position(mGraphicalModelApi.graphicalIdsByLogicalId(relationship).first());
		Id createdGraphicalRelationshipId2 = mGraphicalModelApi.createElement(createdGraphicalDiagramId, createdLogicalRelationshipId2, true, QString("Many-to-many"), QPointF());

		mLogicalModelApi.setPropertyByRoleName(createdLogicalRelationshipId, relationshipName, "Name");

		mGraphicalModelApi.setFrom(createdGraphicalRelationshipId2, createdGraphicalTableId);
		mGraphicalModelApi.setTo(createdGraphicalRelationshipId2,  mGraphicalModelApi.graphicalIdsByLogicalId(setTables.at(toSet)).first());
		mLogicalModelApi.setFrom(newRelationshipId, createdLogicalTableId);
		mLogicalModelApi.setTo(newRelationshipId, setTables.at(toSet));

		//add bounding attribute
		Id rowId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Row"));
		Id createdLogicalRowId = mLogicalModelApi.createElement(Id::rootId(), rowId);
		QPointF rowCoordinates = mGraphicalModelApi.position(createdGraphicalTableId);
		mGraphicalModelApi.createElement(createdGraphicalTableId, createdLogicalRowId, true, QString("Row"), rowCoordinates);

		QString rowName = "keyOf" + getProperty(setTables.at(fromSet), "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(createdLogicalRowId, rowName, "Name");

		//add bounding attribute 2
		Id rowId2 = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Row"));
		Id createdLogicalRowId2 = mLogicalModelApi.createElement(Id::rootId(), rowId2);
		QPointF rowCoordinates2 = mGraphicalModelApi.position(createdGraphicalTableId);
		mGraphicalModelApi.createElement(createdGraphicalTableId, createdLogicalRowId2, true, QString("Row"), rowCoordinates);

		QString rowName2 = "keyOf" + getProperty(setTables.at(toSet), "Name").toString();
		mLogicalModelApi.setPropertyByRoleName(createdLogicalRowId2, rowName2, "Name");

		/*QString toPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(toSet));
		Id fromPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(fromSet));
		QString fromPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(fromSet));
		codeFile.write((toPrimaryKeyName + " " + getProperty(toPrimaryKey, "DataType").toByteArray()).toUtf8());
		codeFile.write(",\r\n");
		codeFile.write((fromPrimaryKeyName + " " + getProperty(fromPrimaryKey, "DataType").toByteArray()).toUtf8());*/
	}

	for (int i = 0; i < oneToOneSetsSize; i++) {
		delete[] match[i];
	}
	delete[] match;
	mErrorReporter->addInformation(tr("Code was generated successfully"));
}
