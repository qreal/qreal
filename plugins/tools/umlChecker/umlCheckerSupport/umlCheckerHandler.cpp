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

#include "umlCheckerHandler.h"

#include <qrkernel/settingsManager.h>

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>
#include <QDir>

using namespace qReal;

UmlCheckerHandler::UmlCheckerHandler(qrRepo::RepoApi *umlPerfectRepoApi
		, qrRepo::RepoApi *umlOrdinaryRepoApi)
		: mPerfectRepoApi(umlPerfectRepoApi)
		, mOrdinaryRepoApi(umlOrdinaryRepoApi)
{
	mQRealSourceFilesPath = "/home/julia/qreal/qreal";
	mPathToPerfect = mQRealSourceFilesPath + "/plugins/tools/umlChecker/perfect/";
	mPathToOrdinary = mQRealSourceFilesPath + "/plugins/tools/umlChecker/ordinary/";
	mPathToPerfectList = mPathToPerfect + "list/";

	mPerfectRepoFromList = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/perfect/list", true);
	QDir dir(mPathToPerfectList);
	QStringList filters;
	filters << "*.qrs";
	dir.setNameFilters(filters);
	mPerfectFileNames = dir.entryList();
	init();
}

UmlCheckerHandler::~UmlCheckerHandler()
{
}

void UmlCheckerHandler::init()
{
	const QString umlSolutionsPath = mPathToOrdinary + "check.qrs";
	const QString umlPerfectSolutionPath = mPathToPerfect + "good.qrs";
	mOrdinaryRepoApi->open(umlSolutionsPath);
	mPerfectRepoApi->open(umlPerfectSolutionPath);
	initBlockNames();
}

QStringList UmlCheckerHandler::initBlockNames()
{
	QStringList result;

	IdList graphicalElements = mPerfectRepoApi->graphicalElements();

	for (const Id &id : graphicalElements) {
		QMap<QString, QVariant> properties = mPerfectRepoApi->properties(id);
		for (const QString &key : properties.keys()) {
			if (key.contains("block") && !mBlockNames.contains(key)) {
				mBlockNames.append(key);
			}
		}
	}

	return result;
}

void UmlCheckerHandler::addBlockName(const QString &blockName)
{
	mBlockNames.append(blockName);
}


void UmlCheckerHandler::saveSolution()
{
	mPerfectRepoApi->save(mPerfectRepoApi->graphicalElements());
}


bool UmlCheckerHandler::matchingLinksOfNode(const IdList &perfectLinks, const IdList &ordinaryLinks)
{
	IdList changeblePerfectLinks = perfectLinks;
	IdList changebleOrdinaryLinks = ordinaryLinks;

	for (const Id &perfectLink : changeblePerfectLinks) {
		for (const Id &ordinaryLink : changebleOrdinaryLinks) {
			if (perfectLink.element() == ordinaryLink.element()) {
				changeblePerfectLinks.removeOne(perfectLink);
				changebleOrdinaryLinks.removeOne(ordinaryLink);
			}
		}
	}

	return changeblePerfectLinks.count() == 0 && changebleOrdinaryLinks.count() == 0;
}


bool UmlCheckerHandler::matchNodeElement(const Id &id, IdList &ordinaryValues)
{
	const IdList incoming = mPerfectRepoApi->incomingLinks(id);
	const IdList outgoing = mPerfectRepoApi->outgoingLinks(id);

	for (const Id &ordinaryId : ordinaryValues) {
		const IdList incomingOrdinary = mOrdinaryRepoApi->incomingLinks(ordinaryId);
		const IdList outgoingOrdinary = mOrdinaryRepoApi->outgoingLinks(ordinaryId);
		bool incomingMatch = matchingLinksOfNode(incoming, incomingOrdinary);
		bool outgoingMatch = matchingLinksOfNode(outgoing, outgoingOrdinary);
		if (incomingMatch && outgoingMatch) {
			ordinaryValues.removeOne(ordinaryId);
			return true;
		}
	}

	return false;
}

IdList UmlCheckerHandler::doShift(const IdList &list)
{
	IdList result = IdList();
	result = list;
	result.removeFirst();
	result.append(list.at(0));
	return result;
}


bool UmlCheckerHandler::checkMatchingNodes(IdList &perfectValues, IdList &ordinaryValues)
{
	IdList changeablePerfect = perfectValues;
	IdList changeableOrdinary = ordinaryValues;

	IdList lastShiftIds = changeablePerfect;

	for (int i = 0; i < perfectValues.size(); ++i) {
		for (const Id &id : changeablePerfect) {
			bool nodeMatch = matchNodeElement(id, changeableOrdinary);
			if (!nodeMatch) {
				changeablePerfect = doShift(lastShiftIds);
				break;
			}

			changeablePerfect.removeOne(id);
		}
	}

	ordinaryValues = changeableOrdinary;
	return ordinaryValues.count() == changeablePerfect.count() && changeablePerfect.count() == 0;
}

bool UmlCheckerHandler::matchingInsideABlock(QMultiHash<QString, Id> perfectElements
		, QMultiHash<QString, Id> &ordinaryElements)
{
	const QStringList perfectKeys = perfectElements.uniqueKeys();

	for (const QString &pKey : perfectKeys) {
		IdList ordinaryValues = ordinaryElements.values(pKey);
		IdList perfectValues = perfectElements.values(pKey);

		if (pKey == "UmlClass") {
			bool resOfCheckMatching = checkMatchingNodes(perfectValues, ordinaryValues);
			if (resOfCheckMatching == false) {
				return resOfCheckMatching;
			}
		}

		ordinaryElements.remove(pKey);
	}

	return true;
}


bool UmlCheckerHandler::matchingResult()
{
	for (const QString &fileName : mPerfectFileNames) {
		mPerfectRepoFromList->open(mPathToPerfectList + fileName);
		const QHash<QString, QMultiHash<QString, Id>> perfectBlocks = getElementsAsBlocks(mPerfectRepoFromList);
		QMultiHash<QString, Id> ordinaryElements = getElements("ordinary");
		const QStringList keys = perfectBlocks.keys();
		for (const QString &key : keys) {
			QMultiHash<QString, Id> perfectElements = perfectBlocks.value(key);
			bool matchingBlock = matchingInsideABlock(perfectElements, ordinaryElements);
			if (!matchingBlock) {
				break;
			}
		}

		if (ordinaryElements.size() == 0) {
			return true;
		}
	}

	return false;
}

QMultiHash<QString, Id> UmlCheckerHandler::getElements(const QString &typeSolution) const
{
	if (typeSolution.contains("perfect")) {
		return getElementsFromApi(mPerfectRepoApi);
	}

	return getElementsFromApi(mOrdinaryRepoApi);
}

QHash<QString, QMultiHash<QString, Id>> UmlCheckerHandler::getElementsAsBlocks(qrRepo::RepoApi *repoApi)
{
	QHash<QString, QMultiHash<QString, Id>> result;

	for (const QString &blockName : mBlockNames) {
		const IdList blockList = repoApi->elementsByProperty(blockName, false, true);
		QMultiHash<QString, Id> hashElements;
		for (const Id &id : blockList) {
			hashElements.insertMulti(id.element(), id);
		}

		result.insert(blockName, hashElements);
	}

	return result;
}

QMultiHash<QString, Id> UmlCheckerHandler::getElementsFromApi(qrRepo::RepoApi *repoApi) const
{
	QMultiHash<QString, Id> hashElements;
	IdList const elements = repoApi->graphicalElements();

	for (const Id &element : elements) {
		if (element.element() == "ClassDiagramNode") {
			IdList list = repoApi->children(element);
			for (const Id &id : list) {
				hashElements.insertMulti(id.element(), id);
			}
		}
	}

	return hashElements;
}
