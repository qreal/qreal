#include "graphTransformationUnit.h"

using namespace qReal;

GraphTransformationUnit::GraphTransformationUnit(qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
{
}

GraphTransformationUnit::~GraphTransformationUnit()
{
}

void GraphTransformationUnit::loadSemantics()
{
	Id const activeDiagram = mInterpretersInterface.activeDiagram();
	IdList const rules = mGraphicalModelApi.graphicalRepoApi().children(activeDiagram);
	
	mRules = new QMap<QString, Id>();
	mDeletedElements = new QMap<QString, QList<Id>* >();
	mReplacedElements = new QMap<QString, QList<Id>* >();
	mCreatedElements = new QMap<QString, QList<Id>* >();
	mElementsWithNewControlMark = new QMap<QString, QList<Id>* >();
	mElementsWithControlMark = new QMap<QString, QList<Id>* >();
	
	foreach (Id const &rule, rules) {
		QString ruleName = getProperty(rule, "ruleName").toString();
		mRules->insert(ruleName, rule);
		IdList const children = mGraphicalModelApi.graphicalRepoApi().children(rule);
		foreach (Id const &child, children) {
			if (child.element() == "ControlFlowLocation") {
				continue;
			}
			
			if (child.element() == "Replacement"){
				Id const from = mLogicalModelApi.logicalRepoApi().from(child);
				putIdIntoMap(mReplacedElements, ruleName, from);
				continue;
			}
			
			QString const semanticsStatus = getProperty(child, "semanticsStatus").toString();
			if (child.element() == "ControlFlowMark") {
				if (semanticsStatus == "" || semanticsStatus == "@deleted@") {
					Id const nodeWithControl = getNodeIdWithControlMark(child);
					putIdIntoMap(mElementsWithControlMark, ruleName, nodeWithControl);
					if (semanticsStatus == "@deleted@") {
						putIdIntoMap(mDeletedElements, ruleName, child);
					}
				} else {
					putIdIntoMap(mCreatedElements, ruleName, child);
				}
				continue;
			}

			if (semanticsStatus == "@new@") {
				putIdIntoMap(mCreatedElements, ruleName, child);
			} else if (semanticsStatus == "@deleted@") {
				putIdIntoMap(mDeletedElements, ruleName, child);
			}
		}
	}
}

void GraphTransformationUnit::interpret()
{
	findMatch();
	makeStep();
}

void GraphTransformationUnit::findMatch()
{
	foreach (QString const &ruleName, mRules->keys()) {
		if (checkOneRuleForMatch(mRules->value(ruleName))) {
			return;
		}
	}
}

bool GraphTransformationUnit::checkOneRuleForMatch(Id const &rule)
{
}

void GraphTransformationUnit::makeStep()
{
}

QVariant GraphTransformationUnit::getProperty(Id const &id, QString const &propertyName)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().property(
					mGraphicalModelApi.logicalId(id), propertyName);
	}
}

void GraphTransformationUnit::putIdIntoMap(QMap<QString ,QList<Id>* > *map,
		QString const &ruleName, Id const &id)
{
	if (!map->contains(ruleName)) {
		map->insert(ruleName, new QList<Id>());
	}
	map->value(ruleName)->append(id);
}

Id GraphTransformationUnit::getNodeIdWithControlMark(Id const &controlMarkId) const
{
	Id const link = mLogicalModelApi.logicalRepoApi().outgoingLinks(controlMarkId).at(0);
	return mLogicalModelApi.logicalRepoApi().to(link);
}
