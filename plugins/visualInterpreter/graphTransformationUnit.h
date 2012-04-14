#pragma once

#include "../../../qrgui/mainwindow/errorReporter.h"
#include "../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "ruleParser.h"

namespace qReal {

/// Graph transformation unit performs different transformation of graphic model
/// according to contained rules which can contain creation, removal, replacement
/// (etc) of elements.
class GraphTransformationUnit : public QObject
{
	Q_OBJECT

public:
	GraphTransformationUnit(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);
	~GraphTransformationUnit();
	
	/// Load semantics model from current open diagram
	void loadSemantics();
	
	/// Make one step according to semantics (find match, delete,
	/// create and replace elements)
	void interpret();
	
private:
	/// For debug uses only
	void highlightMatch();
	
	/// Checks current diagram for being semantics model
	bool isSemanticsEditor();
	
	/// True if match was found
	bool findMatch();
	
	/// Fixes first element and starts checking process
	bool checkRuleMatching(Id const &rule);
	
	/// Checks rule matching recursively accordingly to given initial data
	bool checkRuleMatchingRecursively();
	
	/// Checks can given node in model be added to match as corresponding node
	/// to given node in rule at curren step
	bool checkNodeForAddingToMatch(Id const &nodeInModel, Id const &nodeInRule);
	
	/// Checks if node in model has all links to the current matched subgraph
	/// as node in rule has
	bool checkExistingLinks(Id const &nodeInModel, Id const &nodeInRule,
			QHash<Id, Id> *linksToAddInMatch);
	
	/// Performs a rollback in match algo, removes last added node to the current
	/// matched subgraph.
	void rollback();
	
	/// Get link from node in rule which leads outside current matched graph
	Id getOutsideLink(Id const &nodeInRule) const;
	
	/// Get second link end
	Id getLinkEnd(Id const &linkInRule, Id const &nodeInRule) const;
	
	/// Returns link id in model which has one of its ends given node in model
	/// and correspond to link in rule and its ends and 
	/// returns root id if it can not be found
	Id getProperLink(Id const &nodeInModel, Id const &linkInRule,
			Id const &linkEndInRule) const;
	
	/// Get all links from given node in model which can correspond
	/// given link in rule
	IdList getProperLinks(Id const &nodeInModel, Id const &linkInRule) const;
	
	/// Get all links from given node in rule to current matched subgraph
	IdList getLinksToMatchedSubgraph(Id const &nodeInRule) const;
	
	/// Perform all transformations
	void makeStep();
	
	/// Fill rules information with this
	void putIdIntoMap(QHash<QString ,IdList*> *map, QString const &ruleName,
			Id const &id);
	
	/// Obtain an element id with the corresponding control flow mark
	Id getNodeIdWithControlMark(Id const &controlMarkId) const;
	
	/// Get all elements from active diagram
	IdList getElementsFromActiveDiagram() const;
	
	/// Get all rules from semantics model
	IdList getRules() const;
	
	/// Get first node from rule to start the algo
	Id getStartElement(Id const &rule) const;
	
	/// Functions for working with properties of elements on model
	QVariant getProperty(Id const &id, QString const &propertyName) const;
	bool hasProperty(Id const &id, QString const &propertyName) const;
	void setProperty(Id const &id, QString const &propertyName,
			QVariant const &value) const;
	QHash<QString, QVariant> getProperties(Id const &id) const;
	
	/// Functions for test elements for equality
	bool compareLinks(Id const &first, Id const &second) const;
	bool compareElements(Id const &first, Id const &second) const;
	bool compareElementTypesAndProperties(Id const &first, Id const &second) const;
	
	bool isEdge(Id const &element) const;
	
	/// Logical repo api methods for more quick access
	Id to(Id const &id) const;
	Id from(Id const &id) const;
	IdList outgoingLinks(Id const &id) const;
	IdList incomingLinks(Id const &id) const;
	IdList links(Id const &id) const;
	IdList children(Id const &id) const;
	
	/// Reports message to the main system
	void report(QString const &message) const;
	
	/// Stops interpretation for some time in ms
	void pause(int time);
	
	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface const &mGraphicalModelApi;
	
	/// Metamodel name which loaded semantics is for
	QString mMetamodelName;
	
	/// All rules in map with key - rule name and value - rule id on diagram
	QHash<QString, Id> *mRules;
	
	/// All maps below has the format: key - rule name, value - list of ids
	QHash<QString, IdList*> *mDeletedElements;
	QHash<QString, IdList*> *mReplacedElements;
	QHash<QString, IdList*> *mCreatedElements;
	QHash<QString, IdList*> *mNodesWithNewControlMark;
	QHash<QString, IdList*> *mNodesWithDeletedControlMark;
	QHash<QString, IdList*> *mNodesWithControlMark;
	
	/// Match map: key - id in rule diagram, value - id in diagram
	/// which will be transformed
	QHash<Id, Id> *mMatch;
	
	/// Nodes of rule subgraph which matched model subgraph at current step
	IdList mCurrentMatchedGraphInRule;
	
	/// Nodes which have links 
	IdList mNodesHavingOutsideLinks;
	
	/// Current position in mNodesHavingOutsideLinks for quicker access
	int mPos;
	
	QString mMatchedRuleName;
	QString mCurrentRuleName;
	
	/// Nodes of model which have control mark
	IdList mCurrentNodesWithControlMark;
	
	/// Rule parser and interpreter to deal with textual part of rules
	RuleParser *mRuleParser;
};

}
