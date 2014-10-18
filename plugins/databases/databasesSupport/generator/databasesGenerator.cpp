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
			mPassedElements.append(relationship);
			QString relationshipName = getProperty(relationship, "name").toByteArray();
			if (relationshipName == "One-to-one") {
				Id newEntity = mLogicalModelApi.logicalRepoApi().to(relationship);
				mPassedElements.append(newEntity);
				boundedEntities.append(getBoundedWithOneToOneRealationship(newEntity));
			}
		}
	}

	IdList relationshipsIn = mLogicalModelApi.logicalRepoApi().incomingLinks(id);
	foreach (Id const &relationship, relationshipsIn) {
		if (mPassedElements.indexOf(relationship) == -1) {
			mPassedElements.append(relationship);
			QString relationshipName = getProperty(relationship, "name").toByteArray();
			if (relationshipName == "One-to-one") {
				Id newEntity = mLogicalModelApi.logicalRepoApi().from(relationship);
				mPassedElements.append(newEntity);
				boundedEntities.append(getBoundedWithOneToOneRealationship(newEntity));
			}
		}
	}
	return boundedEntities;
}

void DatabasesGenerator::generateSQL()
{
	mErrorReporter->clear();


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


	codeFile.close();
	/*	if (!mDebuggerConnector->hasBuildError()) {
				mErrorReporter->addInformation(tr("Code generated and builded successfully"));
			} */
}

/*void VisualDebugger::generateCode()
{
	QList<Id> passedElements;
	mHasCodeGenerationError = false;

	setCodeFileName(SettingsManager::value("codeFileName").toString());
	setWorkDir(SettingsManager::value("debugWorkingDirectory").toString());

	codeFile.setFileName(mWorkDir + mCodeFileName);
	codeFile.open(QIODevice::WriteOnly);

	printTabs(0);
	codeFile.write("void main(int argc, char* argv[]) {\n");
	tabsCount++;
	Id const startId = findBeginNode("InitialNode");

	if (startId != Id::rootId()) {
		generateCode(startId, codeFile, passedElements);
		printTabs(-1);
		codeFile.write("}");
		codeFile.close();
		return;
	}

	codeFile.close();
	error(codeGenerationError);
	return;
}

void VisualDebugger::getConditionLinks(IdList const &outLinks, Id &falseEdge, Id &trueEdge)
{
	foreach (Id const &outLink, outLinks) {
		if (checkForIncorrectUseOfLink(outLink, "ControlFlow")) {
			error(codeGenerationError);
			return;
		}

		bool type = getProperty(outLink, "condition").toBool();
		if (type) {
			trueEdge = outLink;
		} else {
			falseEdge = outLink;
		}
	}
}

void VisualDebugger::generateCode(Id const &id, QFile &codeFile, QList<Id> passedElements)
{
	if (passedElements.contains(id)) {
		error(incorrectUseOfCycle);
		return;
	}
	passedElements.append(id);
	if (id.element() == "ConditionNode") {
		printTabs(0);
		codeFile.write("if (");
		codeFile.write(getProperty(id, "condition").toByteArray());
		codeFile.write(") {\n");
		tabsCount++;

		IdList const outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
		Id falseEdge = falseEdge.rootId();
		Id trueEdge = trueEdge.rootId();

		getConditionLinks(outLinks, falseEdge, trueEdge);

		if (mHasCodeGenerationError) {
			return;
		}
		if (trueEdge == trueEdge.rootId()) {
			error(VisualDebugger::missingValidLink);
			error(codeGenerationError);
			return;
		}

		generateCode(trueEdge, codeFile, passedElements);
		printTabs(-1);
		codeFile.write("}");

		if (falseEdge != falseEdge.rootId()) {
			codeFile.write(" else {\n");
			tabsCount++;
			generateCode(falseEdge, codeFile, passedElements);
			printTabs(-1);
			codeFile.write("}\n");
		} else {
			codeFile.write("\n");
		}

	} else if (id.element() == "ActionElement") {
		QString const code = getProperty(id, "process").toString();
		printTabs(0);
		if (code.mid(0,4) == "var ") {
			codeFile.write(code.mid(4).toLatin1());
		} else {
			codeFile.write(code.toLatin1());
		}

		codeFile.write("\n");
		if (mLogicalModelApi.logicalRepoApi().outgoingLinks(id).count() != 0) {
			Id const nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(id).at(0);
			if (checkForIncorrectUseOfLink(nextEdge, "ConditionControlFlow")) {
				error(codeGenerationError);
				return;
			}

			generateCode(nextEdge, codeFile, passedElements);
		} else {
			error(VisualDebugger::missingEndNode);
			error(codeGenerationError);
			return;
		}

	} else if (id.element() == "InitialNode") {
		Id const nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(id).at(0);
		if (checkForIncorrectUseOfLink(nextEdge, "ConditionControlFlow")) {
			error(codeGenerationError);
			return;
		}
		generateCode(nextEdge, codeFile, passedElements);
	} else if (id.element() == "BlockFinalNode") {
		return;
	} else {
		Id const nextNode = mLogicalModelApi.logicalRepoApi().to(id);
		generateCode(nextNode, codeFile, passedElements);
	}
}*/






