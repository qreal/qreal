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
{
	//mAppTranslator.load(":/DatabasesGenerator_" + QLocale::system().name());
	//QApplication::installTranslator(&mAppTranslator);

	//mParser = new BlockParser(mErrorReporter);
}

DatabasesGenerator::~DatabasesGenerator()
{
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
	/*switch (error) {
		case missingBeginNode:
			mInterpretersInterface.errorReporter()->addCritical(tr("The diagram doesn't have Initial Node"));
			break;
		case endWithNotEndNode:
			mInterpretersInterface.errorReporter()->addWarning(
					tr("There are no links from this node and it mismatches Final Node")
					, mCurrentId);
			break;
		case noErrors:
			return;
			break;
	}*/
	mCurrentId = Id::rootId();
	mCurrentDiagram = Id::rootId();
}

Id DatabasesGenerator::getPrimaryKey(Id const &entityId)
{
	IdList attributesSet = getChildren(entityId);
	/*if(attributesSet.isEmpty()) {
		error(emptyEntity);
	}*/
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
		error("Invalid number of primary key in entity " + getProperty(entityId, "isPrimaryKey").toString() +  ": " + primaryKeyCount, true);
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
	foreach (IdList const &list, set) {
		if (list.indexOf(childEntity) != -1)
			return list.indexOf(childEntity);
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
			name += getProperty(id, "Name").toByteArray();
		}
	}
	return name;
}

void DatabasesGenerator::generateSQL()
{
	mErrorReporter->clear();
	//error(missingBeginNode);


	mPassedElements.clear();

	//setCodeFileName(SettingsManager::value("databasesCodeFileName").toString());

	codeFile.setFileName(mWorkDir + mCodeFileName);
	codeFile.open(QIODevice::WriteOnly);

	IdList entityNodes = findNodes("Entity");
	//IdList attributeNodes = findNodes("Attribute");

	QList<IdList> oneToOneAllTablesSet;
	oneToOneAllTablesSet.clear();

	foreach (Id const &entityId, entityNodes) {
		IdList relationships = mLogicalModelApi.logicalRepoApi().outgoingLinks(entityId);
		relationships.append(mLogicalModelApi.logicalRepoApi().incomingLinks(entityId));

		if (relationships.isEmpty()) {
			mPassedElements.append(entityId);
			codeFile.write("CREATE TABLE ");
			codeFile.write(getProperty(entityId, "Name").toByteArray());
			codeFile.write("\r\n(");
			IdList attributesSet = getChildren(entityId);

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

				/*foreach (Id const &relationship, relationships) {
					QString relationshipName = getProperty(relationship, "name").toByteArray();
					if (relationshipName == "One-to-one") {
						Id c = mLogicalModelApi.logicalRepoApi().to(relationship);
					}
				}*/

				oneToOneAllTablesSet.append(oneToOneTableSet);
			}
		}
	}

	//QList<int> passedLists;
	//passedLists.clear();
	QString* extraAttributes = new QString[oneToOneAllTablesSet.size()];
	for(int i = 0; i < oneToOneAllTablesSet.size(); i++) {
		extraAttributes[i] = "";
	}


	IdList oneToManyRelationships = findNodes("One-to-many");
	foreach (Id const &relationship, oneToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneAllTablesSet);
		int fromSet = getParentList(from, oneToOneAllTablesSet);

		Id toPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(toSet));
		QString toPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(toSet));
		extraAttributes[fromSet] += (",\r\n" + toPrimaryKeyName + " " + getProperty(toPrimaryKey, "DataType").toString());
	}

	IdList manyToManyRelationships = findNodes("Many-to-many");
	foreach (Id const &relationship, manyToManyRelationships) {
		Id to = mLogicalModelApi.logicalRepoApi().to(relationship);
		Id from = mLogicalModelApi.logicalRepoApi().from(relationship);
		int toSet = getParentList(to, oneToOneAllTablesSet);
		int fromSet = getParentList(from, oneToOneAllTablesSet);

		Id toPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(fromSet));
		QString toPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(fromSet));
		Id fromPrimaryKey = getPrimaryKeyOfSet(oneToOneAllTablesSet.at(fromSet));
		QString fromPrimaryKeyName = getPrimaryKeyNameOfSet(oneToOneAllTablesSet.at(fromSet));

		codeFile.write("CREATE TABLE ");
		codeFile.write((getListTableName(oneToOneAllTablesSet.at(fromSet)) + "-" + getListTableName(oneToOneAllTablesSet.at(toSet))).toUtf8());
		codeFile.write("\r\n(");
		codeFile.write("\r\n");

		codeFile.write((toPrimaryKeyName + " " + getProperty(toPrimaryKey, "DataType").toByteArray()).toUtf8());
		codeFile.write(",\r\n");
		codeFile.write((fromPrimaryKeyName + " " + getProperty(fromPrimaryKey, "DataType").toByteArray()).toUtf8());
		codeFile.write("\r\n);\r\n\r\n");
	}

	int i = 0;
	foreach (IdList const &list, oneToOneAllTablesSet) {
		codeFile.write("CREATE TABLE ");
		codeFile.write(getListTableName(list).toUtf8());
		codeFile.write("\r\n(");
		codeFile.write("\r\n");

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
		}
		codeFile.write(extraAttributes[i].toUtf8());
		i++;


		//codeFile.write(toPrimaryKeyName + " " + getProperty(toPrimaryKey, "DataType").toByteArray(););
		//codeFile.write(",\r\n");

		codeFile.write("\r\n);\r\n\r\n");
	}
	codeFile.close();
}

