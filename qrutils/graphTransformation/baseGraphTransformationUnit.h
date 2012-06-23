#pragma once

#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

/// Graph transformation unit performs different transformation of graphic model
/// according to contained rules which can contain creation, removal, replacement
/// (etc) of elements.
class BaseGraphTransformationUnit : public QObject
{
	Q_OBJECT

public:
	BaseGraphTransformationUnit(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);
	~BaseGraphTransformationUnit();

	/// True if match was found
	virtual bool findMatch() = 0;
	
	/// Get all matches of selected rule
	QList<QHash<Id, Id> > getMatches();
	
protected:
	
	/// Fixes first element and starts checking process
	bool checkRuleMatching();
	
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
	Id getLinkEndInModel(Id const &linkInModel, Id const &nodeInModel) const;
	Id getLinkEndInRule(Id const &linkInRule, Id const &nodeInRule) const;
	
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
	
	/// Get all elements from active diagram
	IdList getElementsFromActiveDiagram() const;
	
	/// Get first node from rule to start the algo
	virtual Id getStartElement() const = 0;
	
	/// Functions for working with properties of elements on model
	QVariant getProperty(Id const &id, QString const &propertyName) const;
	virtual QMapIterator<QString, QVariant> getPropertiesIterator(Id const &id) const;
	bool hasProperty(Id const &id, QString const &propertyName) const;
	void setProperty(Id const &id, QString const &propertyName,
			QVariant const &value) const;
	QHash<QString, QVariant> getProperties(Id const &id) const;
	
	/// Functions for test elements for equality
	virtual bool compareLinks(Id const &first, Id const &second) const;
	virtual bool compareElements(Id const &first, Id const &second) const;
	virtual bool compareElementTypesAndProperties(Id const &first, Id const &second) const;
	
	bool isEdgeInModel(Id const &element) const;
	bool isEdgeInRule(Id const &element) const;
	
	/// Logical repo api methods for more quick access
	Id toInModel(Id const &id) const;
	Id fromInModel(Id const &id) const;
	IdList linksInModel(Id const &id) const;
	virtual Id toInRule(Id const &id) const;
	virtual Id fromInRule(Id const &id) const;
	virtual IdList linksInRule(Id const &id) const = 0;
	
	IdList outgoingLinks(Id const &id) const;
	IdList incomingLinks(Id const &id) const;
	IdList children(Id const &id) const;
	
	/// Reports message to the main system
	void report(QString const &message) const;
	
	/// Hold highligth for some time in ms
	void pause(int const &time);
	
	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface const &mGraphicalModelApi;
	
	Id ruleToFind;
	
	/// Match map: key - id in rule diagram, value - id in diagram
	/// which will be transformed
	QHash<Id, Id> *mMatch;
	
	/// List contains all matches of rule
	QList<QHash<Id, Id> > mMatches;
	
	/// Nodes of rule subgraph which matched model subgraph at current step
	IdList mCurrentMatchedGraphInRule;
	
	/// Nodes which have links 
	IdList mNodesHavingOutsideLinks;
	
	/// Current position in mNodesHavingOutsideLinks for quicker access
	int mPos;
	
	/// Set of properties that will not be checked in compare elements
	QSet<QString> defaultProperties;
};

}
