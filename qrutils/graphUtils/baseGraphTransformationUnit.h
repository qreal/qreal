/* Copyright 2007-2015 QReal Research Group
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

#pragma once

#include "qrutils/utilsDeclSpec.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

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
	bool checkRuleMatching(const IdList &elements);

	/// Checks rule matching recursively accordingly to given initial data
	bool checkRuleMatchingRecursively();

	/// Checks can given node in model be added to match as corresponding node
	/// to given node in rule at curren step
	bool checkNodeForAddingToMatch(const Id &nodeInModel, const Id &nodeInRule);

	/// Checks if node in model has all links to the current matched subgraph
	/// as node in rule has
	bool checkExistingLinks(const Id &nodeInModel, const Id &nodeInRule
			, QHash<Id, Id> *linksToAddInMatch);

	/// Performs a rollback in match algo, removes last added node to the current
	/// matched subgraph.
	void rollback();

	/// Get link from node in rule which leads outside current matched graph
	Id outsideLink(const Id &nodeInRule) const;

	/// Get second link end
	Id linkEndInModel(const Id &linkInModel, const Id &nodeInModel) const;
	Id linkEndInRule(const Id &linkInRule, const Id &nodeInRule) const;

	/// Returns link id in model which has one of its ends given node in model
	/// and correspond to link in rule and its ends and
	/// returns root id if it can not be found
	Id properLink(const Id &nodeInModel, const Id &linkInRule
			, const Id &linkEndInR) const;

	/// Get all links from given node in model which can correspond
	/// given link in rule
	IdList properLinks(const Id &nodeInModel, const Id &linkInRule) const;

	/// Get all links from given node in rule to current matched subgraph
	IdList linksToMatchedSubgraph(const Id &nodeInRule) const;

	/// Get all elements from active diagram
	IdList elementsFromActiveDiagram() const;

	/// Get first node from rule to start the algo
	virtual Id startElement() const = 0;

	/// Indicates if checked rule has syntax errors
	bool hasRuleSyntaxError();

	/// Resets to false rule syntax error indicator
	void resetRuleSyntaxCheck();

	/// Functions for working with properties of elements on model
	QVariant property(const Id &id, const QString &propertyName) const;
	virtual QMapIterator<QString, QVariant> propertiesIterator(const Id &id) const;
	bool hasProperty(const Id &id, const QString &propertyName) const;
	void setProperty(const Id &id, const QString &propertyName
			, const QVariant &value) const;
	QHash<QString, QVariant> properties(const Id &id) const;

	/// Functions for test elements for equality
	virtual bool compareLinks(const Id &first, const Id &second) const;
	virtual bool compareElements(const Id &first, const Id &second) const;
	virtual bool compareElementTypesAndProperties(const Id &first, const Id &second) const;

	bool isEdgeInModel(const Id &element) const;
	bool isEdgeInRule(const Id &element) const;

	/// Logical repo api methods for more quick access
	Id toInModel(const Id &id) const;
	Id fromInModel(const Id &id) const;
	IdList linksInModel(const Id &id) const;
	virtual Id toInRule(const Id &id) const;
	virtual Id fromInRule(const Id &id) const;
	virtual IdList linksInRule(const Id &id) const;

	IdList outgoingLinks(const Id &id) const;
	IdList incomingLinks(const Id &id) const;
	IdList children(const Id &id) const;

	/// Reports message to the main system
	virtual void report(const QString &message, bool isError) const;

	/// Hold highlight for some time in ms
	void pause(const int &time);

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
