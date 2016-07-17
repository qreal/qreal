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

#include "baseGraphTransformationUnit.h"

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

using namespace qReal;

BaseGraphTransformationUnit::BaseGraphTransformationUnit(
		qReal::LogicalModelAssistInterface &logicalModelApi
		, qReal::GraphicalModelAssistInterface &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mHasRuleSyntaxErr(false)
{
	mDefaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to"
			<< "fromPort" << "toPort");
}

BaseGraphTransformationUnit::~BaseGraphTransformationUnit()
{
}

IdList BaseGraphTransformationUnit::elementsFromActiveDiagram() const
{
	const unsigned validIdSize = 4;
	const Id activeDiagram = mInterpretersInterface.activeDiagram();
	if (activeDiagram.idSize() < validIdSize) {
		mInterpretersInterface.errorReporter()->addError(tr("no current diagram"));
		return IdList();
	}
	const IdList activeDiagramElements = children(activeDiagram);
	IdList activeDiagramGraphicalElements;
	foreach (const Id &id, activeDiagramElements) {
		if (mGraphicalModelApi.isGraphicalId(id)) {
			activeDiagramGraphicalElements.append(id);
		}
	}
	return activeDiagramGraphicalElements;
}

bool BaseGraphTransformationUnit::checkRuleMatching()
{
	const IdList elements = elementsFromActiveDiagram();

	return checkRuleMatching(elements);
}

bool BaseGraphTransformationUnit::checkRuleMatching(const IdList &elements)
{
	mMatch = QHash<Id, Id>();
	bool isMatched = false;

	const Id startElem = startElement();
	if (startElem == Id::rootId()) {
		report(tr("Rule '") + property(mRuleToFind, "ruleName").toString()
				+ tr("' has not any appropriate nodes"), true);

		mHasRuleSyntaxErr = true;
		return false;
	}

	mNodesHavingOutsideLinks.append(startElem);

	foreach (const Id &element, elements) {
		if (compareElements(element, startElem)) {
			mCurrentMatchedGraphInRule.clear();
			mCurrentMatchedGraphInModel.clear();
			mNodesHavingOutsideLinks.clear();
			mMatch.clear();
			mPos = 0;

			mCurrentMatchedGraphInRule.append(startElem);
			mCurrentMatchedGraphInModel.append(element);
			mNodesHavingOutsideLinks.append(startElem);

			mMatch.insert(startElem, element);

			if (checkRuleMatchingRecursively()) {
				isMatched = true;
			}
		}
	}

	return isMatched;
}

bool BaseGraphTransformationUnit::checkRuleMatchingRecursively()
{
	if (mNodesHavingOutsideLinks.length() == mPos) {
		QHash<Id, Id> currentMatch = mMatch;
		mMatches.append(currentMatch);
		return true;
	}

	bool isMatched = false;

	const Id nodeInRule = mNodesHavingOutsideLinks.at(mPos);
	const Id linkInRule = outsideLink(nodeInRule);

	if (linkInRule != Id::rootId()) {
		const Id linkEndInRuleElement = linkEndInRule(linkInRule, nodeInRule);
		if (linkEndInRuleElement == Id::rootId()) {
			report(tr("Rule '") + property(mRuleToFind, "ruleName").toString() + tr("' has unconnected link"), true);
			mHasRuleSyntaxErr = true;
			return false;
		}

		const Id nodeInModel = mMatch.value(nodeInRule);
		const IdList linksInModel = properLinks(nodeInModel, linkInRule);

		QHash<Id, Id> matchBackup = mMatch;
		IdList nodesHavingOutsideLinksBackup = mNodesHavingOutsideLinks;
		IdList currentMatchedGraphInRuleBackup = mCurrentMatchedGraphInRule;
		IdList currentMatchedGraphInModelBackup = mCurrentMatchedGraphInModel;
		const int posBackup = mPos;

		foreach (const Id &linkInModel, linksInModel) {
			const Id linkEndInModelElement = linkEndInModel(linkInModel, nodeInModel);
			if (checkNodeForAddingToMatch(linkEndInModelElement, linkEndInRuleElement)) {
				if (checkRuleMatchingRecursively()) {
					isMatched = true;
					mMatch = QHash<Id, Id>();
					foreach (const Id id, matchBackup.keys()) {
						mMatch.insert(id, matchBackup.value(id));
					}

					mNodesHavingOutsideLinks = IdList();
					foreach (const Id id, nodesHavingOutsideLinksBackup) {
						mNodesHavingOutsideLinks.append(id);
					}
					mCurrentMatchedGraphInRule = IdList();
					foreach (const Id id, currentMatchedGraphInRuleBackup) {
						mCurrentMatchedGraphInRule.append(id);
					}
					mCurrentMatchedGraphInModel = IdList();
					foreach (const Id id, currentMatchedGraphInModelBackup) {
						mCurrentMatchedGraphInModel.append(id);
					}

					mPos = posBackup;
				} else {
					rollback();
				}
			}
		}
		return isMatched;
	} else {
		mPos++;
		return checkRuleMatchingRecursively();
	}
}

bool BaseGraphTransformationUnit::checkNodeForAddingToMatch(const Id &nodeInModel, const Id &nodeInRule)
{
	if (nodeInModel == Id::rootId()) {
		return false;
	}

	QHash<Id, Id> *linksToAddToMatch = new QHash<Id, Id>();

	bool res = checkExistingLinks(nodeInModel, nodeInRule, linksToAddToMatch);

	if (res) {
		mMatch.unite(*linksToAddToMatch);
		mMatch.insert(nodeInRule, nodeInModel);
		mCurrentMatchedGraphInRule.append(nodeInRule);
		mCurrentMatchedGraphInModel.append(nodeInModel);
		mNodesHavingOutsideLinks.append(nodeInRule);
	}

	return res;
}

bool BaseGraphTransformationUnit::checkExistingLinks(const Id &nodeInModel
		, const Id &nodeInRule, QHash<Id, Id> *linksToAddInMatch)
{
	const IdList linksInRuleElement = linksInRule(nodeInRule);

	foreach (const Id &linkInRule, linksInRuleElement) {
		const Id linkEndInR = linkEndInRule(linkInRule, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEndInR)) {
			Id properLinkInModel = properLink(nodeInModel, linkInRule, linkEndInR);
			if (properLinkInModel == Id::rootId()) {
				return false;
			} else {
				if (mLogicalModelApi.logicalRepoApi().isLogicalElement(properLinkInModel)) {
					const IdList properGraphicalLinks = mGraphicalModelApi.graphicalIdsByLogicalId(properLinkInModel);
					if (!properGraphicalLinks.isEmpty()) {
						properLinkInModel = properGraphicalLinks.first();
					}
				}
				linksToAddInMatch->insert(linkInRule, properLinkInModel);
			}
		}
	}

	return true;
}

void BaseGraphTransformationUnit::rollback()
{
	const Id nodeToRemove = mCurrentMatchedGraphInRule.last();

	mMatch.remove(nodeToRemove);
	mCurrentMatchedGraphInRule.removeLast();
	mCurrentMatchedGraphInModel.removeLast();
	mNodesHavingOutsideLinks.removeLast();
	if (mPos == mNodesHavingOutsideLinks.length()) {
		mPos--;
	}

	const IdList links = linksToMatchedSubgraph(nodeToRemove);
	foreach (const Id &link, links) {
		mMatch.remove(link);
	}
}

IdList BaseGraphTransformationUnit::linksToMatchedSubgraph(const Id &nodeInRule) const
{
	IdList result;
	const IdList lnksInRule = linksInRule(nodeInRule);
	foreach (const Id &link, lnksInRule) {
		const Id linkEnd = linkEndInRule(link, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEnd)) {
			result.append(link);
		}
	}
	return result;
}

Id BaseGraphTransformationUnit::outsideLink(const Id &nodeInRule) const
{
	const IdList lnksInRule = linksInRule(nodeInRule);
	foreach (const Id &linkInRule, lnksInRule) {
		const Id linkEndInR = linkEndInRule(linkInRule, nodeInRule);
		if (!mCurrentMatchedGraphInRule.contains(linkEndInR)) {
			return linkInRule;
		}
	}

	return Id::rootId();
}

Id BaseGraphTransformationUnit::linkEndInModel(const Id &linkInModel, const Id &nodeInModel) const
{
	const Id linkTo = toInModel(linkInModel);
	if (linkTo == nodeInModel) {
		return fromInModel(linkInModel);
	}
	return linkTo;
}

Id BaseGraphTransformationUnit::linkEndInRule(const Id &linkInRule, const Id &nodeInRule) const
{
	const Id linkTo = toInRule(linkInRule);
	if (linkTo == nodeInRule) {
		return fromInRule(linkInRule);
	}
	return linkTo;
}

Id BaseGraphTransformationUnit::properLink(const Id &nodeInModel, const Id &linkInRule, const Id &linkEndInR) const
{
	const IdList lnksInModel = linksInModel(nodeInModel);
	foreach (const Id &linkInModel, lnksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			Id linkEndInM = linkEndInModel(linkInModel, nodeInModel);
			if (!mLogicalModelApi.isLogicalId(linkEndInM)) {
				linkEndInM = mGraphicalModelApi.logicalId(linkEndInM);
			}
			Id linkEndInRValue = mMatch.value(linkEndInR);
			if (!mLogicalModelApi.isLogicalId(linkEndInRValue)) {
				linkEndInRValue = mGraphicalModelApi.logicalId(linkEndInRValue);
			}
			if (linkEndInM == linkEndInRValue) {
				return linkInModel;
			}
		}
	}

	return Id::rootId();
}

IdList BaseGraphTransformationUnit::properLinks(const Id &nodeInModel, const Id &linkInRule) const
{
	IdList result;
	const IdList lnksInModel = linksInModel(nodeInModel);
	foreach (const Id &linkInModel, lnksInModel) {
		if (mCurrentMatchedGraphInModel.contains(linkEndInModel(linkInModel, nodeInModel))) {
			continue;
		}

		if (compareLinks(linkInModel, linkInRule)) {
			if (!mLogicalModelApi.isLogicalId(linkInModel)) {
				result.append(linkInModel);
			} else {
				result.append(mGraphicalModelApi.graphicalIdsByLogicalId(linkInModel));
			}
		}
	}
	return result;
}

bool BaseGraphTransformationUnit::compareLinks(const Id &first,const Id &second) const
{
	Id  idTo1 = toInModel(first);
	const Id idTo2 = toInRule(second);
	Id idFrom1 = fromInModel(first);
	const Id idFrom2 = fromInRule(second);

	bool result = compareElementTypesAndProperties(first, second)
			&& compareElements(idTo1, idTo2)
			&& compareElements(idFrom1, idFrom2);

	if (mMatch.contains(idTo2)) {
		Id matchTo = mMatch.value(idTo2);
		if (!mLogicalModelApi.isLogicalId(matchTo)) {
			matchTo = mGraphicalModelApi.logicalId(matchTo);
		}
		if (!mLogicalModelApi.isLogicalId(idTo1)) {
			idTo1 = mGraphicalModelApi.logicalId(idTo1);
		}
		result = result && matchTo == idTo1;
	}

	if (mMatch.contains(idFrom2)) {
		Id matchTo = mMatch.value(idFrom2);
		if (!mLogicalModelApi.isLogicalId(matchTo)) {
			matchTo = mGraphicalModelApi.logicalId(matchTo);
		}
		if (!mLogicalModelApi.isLogicalId(idFrom1)) {
			idFrom1 = mGraphicalModelApi.logicalId(idFrom1);
		}
		result = result && matchTo == idFrom1;
	}

	return result;
}

bool BaseGraphTransformationUnit::compareElements(const Id &first, const Id &second) const
{
	return compareElementTypesAndProperties(first, second);
}

bool BaseGraphTransformationUnit::compareElementTypesAndProperties(const Id &first
		, const Id &second) const
{
	if (first.element() == second.element() && first.diagram() == second.diagram()) {
		QHash<QString, QVariant> secondProperties = properties(second);
		foreach (const QString &key, secondProperties.keys()) {
			const QVariant value = secondProperties.value(key);
			if (value.toString().isEmpty()) {
				continue;
			}

			if (!hasProperty(first, key) || property(first, key) != value) {
				return false;
			}
		}

		return true;
	}

	return false;
}

bool BaseGraphTransformationUnit::hasProperty(const Id &id, const QString &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(mGraphicalModelApi.logicalId(id), propertyName);
	}
}

QHash<QString, QVariant> BaseGraphTransformationUnit::properties(const Id &id) const
{
	QHash<QString, QVariant> res;

	if (id != Id::rootId()) {
		QMapIterator<QString, QVariant> properties = propertiesIterator(id);

		while (properties.hasNext()) {
			properties.next();

			if (!mDefaultProperties.contains(properties.key())) {
				res.insert(properties.key(), properties.value());
			}
		}
	}

	return res;
}

QMapIterator<QString, QVariant> BaseGraphTransformationUnit::propertiesIterator(const Id &id) const
{
	return (mLogicalModelApi.isLogicalId(id))
			? mLogicalModelApi.logicalRepoApi().propertiesIterator(id)
			: mLogicalModelApi.logicalRepoApi().propertiesIterator(mGraphicalModelApi.logicalId(id));
}

QVariant BaseGraphTransformationUnit::property(const Id &id, const QString &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	}
	return mLogicalModelApi.logicalRepoApi().property(mGraphicalModelApi.logicalId(id), propertyName);
}

void BaseGraphTransformationUnit::setProperty(const Id &id, const QString &propertyName
		, const QVariant &value) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		mLogicalModelApi.mutableLogicalRepoApi().setProperty(id, propertyName, value);
	}
	mLogicalModelApi.mutableLogicalRepoApi().setProperty(mGraphicalModelApi.logicalId(id), propertyName, value);
}

bool BaseGraphTransformationUnit::isEdgeInModel(const Id &element) const
{
	return toInModel(element) != Id::rootId() || fromInModel(element) != Id::rootId();
}

bool BaseGraphTransformationUnit::isEdgeInRule(const Id &element) const
{
	return toInRule(element) != Id::rootId() || fromInRule(element) != Id::rootId();
}

Id BaseGraphTransformationUnit::toInModel(const Id &id) const
{
	Id result;
	if (mLogicalModelApi.isLogicalId(id)) {
		result = mLogicalModelApi.logicalRepoApi().to(id);
	} else {
		result = mLogicalModelApi.logicalRepoApi().to(mGraphicalModelApi.logicalId(id));
	}
	if (!mGraphicalModelApi.graphicalIdsByLogicalId(result).isEmpty()) {
		result = mGraphicalModelApi.graphicalIdsByLogicalId(result).first();
	}
	return result;
}

Id BaseGraphTransformationUnit::fromInModel(const Id &id) const
{
	Id result;
	if (mLogicalModelApi.isLogicalId(id)) {
		result = mLogicalModelApi.logicalRepoApi().from(id);
	} else {
		result = mLogicalModelApi.logicalRepoApi().from(mGraphicalModelApi.logicalId(id));
	}
	if (!mGraphicalModelApi.graphicalIdsByLogicalId(result).isEmpty()) {
		result = mGraphicalModelApi.graphicalIdsByLogicalId(result).first();
	}
	return result;
}

Id BaseGraphTransformationUnit::toInRule(const Id &id) const
{
	Id result;
	if (mLogicalModelApi.isLogicalId(id)) {
		result = mLogicalModelApi.logicalRepoApi().to(id);
	} else {
		result = mLogicalModelApi.logicalRepoApi().to(mGraphicalModelApi.logicalId(id));
	}
	if (!mGraphicalModelApi.graphicalIdsByLogicalId(result).isEmpty()) {
		result = mGraphicalModelApi.graphicalIdsByLogicalId(result).first();
	}
	return result;
}

Id BaseGraphTransformationUnit::fromInRule(const Id &id) const
{
	Id result;
	if (mLogicalModelApi.isLogicalId(id)) {
		result = mLogicalModelApi.logicalRepoApi().from(id);
	} else {
		result = mLogicalModelApi.logicalRepoApi().from(mGraphicalModelApi.logicalId(id));
	}
	if (!mGraphicalModelApi.graphicalIdsByLogicalId(result).isEmpty()) {
		result = mGraphicalModelApi.graphicalIdsByLogicalId(result).first();
	}
	return result;
}

IdList BaseGraphTransformationUnit::linksInModel(const Id &id) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().links(id);
	}
	return mLogicalModelApi.logicalRepoApi().links(mGraphicalModelApi.logicalId(id));
}

IdList BaseGraphTransformationUnit::linksInRule(const Id &id) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().links(id);
	}
	return mLogicalModelApi.logicalRepoApi().links(mGraphicalModelApi.logicalId(id));
}

IdList BaseGraphTransformationUnit::outgoingLinks(const Id &id) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
	}
	return mLogicalModelApi.logicalRepoApi().outgoingLinks(mGraphicalModelApi.logicalId(id));
}

IdList BaseGraphTransformationUnit::incomingLinks(const Id &id) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().incomingLinks(id);
	}
	return mLogicalModelApi.logicalRepoApi().incomingLinks(mGraphicalModelApi.logicalId(id));
}

IdList BaseGraphTransformationUnit::children(const Id &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

void BaseGraphTransformationUnit::report(const QString &message, bool isError) const
{
	if (isError) {
		mInterpretersInterface.errorReporter()->addCritical(message);
	} else {
		mInterpretersInterface.errorReporter()->addInformation(message);
	}
}

QList<QHash<Id, Id> > BaseGraphTransformationUnit::matches()
{
	return mMatches;
}

void BaseGraphTransformationUnit::pause(const int &time)
{
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();
}

bool BaseGraphTransformationUnit::hasRuleSyntaxError()
{
	return mHasRuleSyntaxErr;
}

void BaseGraphTransformationUnit::resetRuleSyntaxCheck()
{
	mHasRuleSyntaxErr = false;
}
