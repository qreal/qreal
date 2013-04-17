#pragma once

#include "../utilsDeclSpec.h"

#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

/// Base graph transformation unit can find all matches of specific rule
/// in given graph.
class QRUTILS_EXPORT BaseGraphTransformationUnit : public QObject
{
	Q_OBJECT

public:
	BaseGraphTransformationUnit(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);
	~BaseGraphTransformationUnit();

	/// True if match was found
	virtual bool findMatch() = 0;

	/// Get all matches of selected rule
	QList<QHash<Id, Id> > matches();

protected:

	/// Finds first element and starts checking process
	bool virtual checkRuleMatching();

	/// Finds first element in specified elements and starts checking process
	bool checkRuleMatching(IdList const &elements);

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
	Id outsideLink(Id const &nodeInRule) const;

	/// Get second link end
	Id linkEndInModel(Id const &linkInModel, Id const &nodeInModel) const;
	Id linkEndInRule(Id const &linkInRule, Id const &nodeInRule) const;

	/// Returns link id in model which has one of its ends given node in model
	/// and correspond to link in rule and its ends and
	/// returns root id if it can not be found
	Id properLink(Id const &nodeInModel, Id const &linkInRule,
			Id const &linkEndInR) const;

	/// Get all links from given node in model which can correspond
	/// given link in rule
	IdList properLinks(Id const &nodeInModel, Id const &linkInRule) const;

	/// Get all links from given node in rule to current matched subgraph
	IdList linksToMatchedSubgraph(Id const &nodeInRule) const;

	/// Get all elements from active diagram
	IdList elementsFromActiveDiagram() const;

	/// Get first node from rule to start the algo
	virtual Id startElement() const = 0;

	/// Indicates if checked rule has syntax errors
	bool hasRuleSyntaxError();

	/// Resets to false rule syntax error indicator
	void resetRuleSyntaxCheck();

	/// Functions for working with properties of elements on model
	QVariant property(Id const &id, QString const &propertyName) const;
	virtual QMapIterator<QString, QVariant> propertiesIterator(Id const &id) const;
	bool hasProperty(Id const &id, QString const &propertyName) const;
	void setProperty(Id const &id, QString const &propertyName,
			QVariant const &value) const;
	QHash<QString, QVariant> properties(Id const &id) const;

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
	virtual IdList linksInRule(Id const &id) const;

	IdList outgoingLinks(Id const &id) const;
	IdList incomingLinks(Id const &id) const;
	IdList children(Id const &id) const;

	/// Reports message to the main system
	virtual void report(QString const &message, bool isError) const;

	/// Hold highlight for some time in ms
	void pause(int const &time);

	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface &mLogicalModelApi;
	GraphicalModelAssistInterface &mGraphicalModelApi;

	Id mRuleToFind;

	bool mHasRuleSyntaxErr;

	/// Match map: key - id in rule diagram, value - id in diagram
	/// which will be transformed
	QHash<Id, Id> mMatch;

	/// List contains all matches of rule
	QList<QHash<Id, Id> > mMatches;

	/// Nodes of rule subgraph which matched model subgraph at current step
	IdList mCurrentMatchedGraphInRule;

	/// Nodes of model subgraph which matched model subgraph at current step
	IdList mCurrentMatchedGraphInModel;

	/// Nodes which have links
	IdList mNodesHavingOutsideLinks;

	/// Current position in mNodesHavingOutsideLinks for quicker access
	int mPos;

	/// Set of properties that will not be checked in compare elements
	QSet<QString> mDefaultProperties;
};

}
