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

void DatabasesGenerator::generateSQL()
{
	mErrorReporter->clear();

	//setCodeFileName(SettingsManager::value("databasesCodeFileName").toString());

	codeFile.setFileName(mWorkDir + mCodeFileName);
	codeFile.open(QIODevice::WriteOnly);

	IdList entityNodes = findNodes("Entity");

	foreach (Id const &entityId, entityNodes) {
		codeFile.write("\nCREATE TABLE ");
		QVariant f = getProperty(entityId, "transaction");
		codeFile.write(getProperty(entityId, "Name").toByteArray());
		codeFile.write("\n{\n");
		codeFile.write("\n}\n");
	}


	codeFile.close();
	/*
	//mVisualDebugger->setCurrentDiagram();

	//mVisualDebugger->generateCode();

			if (!mDebuggerConnector->hasBuildError()) {
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






