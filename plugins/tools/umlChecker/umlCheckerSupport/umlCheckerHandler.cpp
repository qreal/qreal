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

using namespace qReal;

UmlCheckerHandler::UmlCheckerHandler(qrRepo::RepoApi *umlPerfectRepoApi
		, qrRepo::RepoApi *umlOrdinaryRepoApi)
		: mPerfectRepoApi(umlPerfectRepoApi)
		, mOrdinaryRepoApi(umlOrdinaryRepoApi)
{
	mQRealSourceFilesPath = "/home/julia/qreal/qreal";
	mPathToPerfect = mQRealSourceFilesPath + "/plugins/tools/umlChecker/perfect/";
	mPathToOrdinary = mQRealSourceFilesPath + "/plugins/tools/umlChecker/ordinary/";

	init();
}

UmlCheckerHandler::~UmlCheckerHandler()
{
}

void UmlCheckerHandler::init()
{
	QString const umlSolutionsPath = mPathToOrdinary + "check.qrs";
	QString const umlPerfectSolutionPath = mPathToPerfect + "good.qrs";
	mOrdinaryRepoApi->open(umlSolutionsPath);
	mPerfectRepoApi->open(umlPerfectSolutionPath);
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


bool UmlCheckerHandler::checkMatchingNodes(const IdList &perfectValues, const IdList &ordinaryValues)
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


	return changeablePerfect.count() == changeablePerfect.count() && changeablePerfect.count() == 0;
}


bool UmlCheckerHandler::matchingResult()
{
	const QMultiHash<QString, Id> perfectElements = getElements("perfect");
	const QMultiHash<QString, Id> ordinaryElements = getElements("ordinary");
	const QStringList keys = perfectElements.uniqueKeys();

	for (const QString &key : keys) {
		const IdList ordinaryValues = ordinaryElements.values(key);
		const IdList perfectValues = ordinaryElements.values(key);

		if (key == "UmlClass") {
			bool resOfCheckMatching = checkMatchingNodes(perfectValues, ordinaryValues);
			if (resOfCheckMatching == false) {
				return resOfCheckMatching;
			}
		}
	}

	return perfectElements.size() == ordinaryElements.size();
}

QMultiHash<QString, Id> UmlCheckerHandler::getElements(const QString &typeSolution) const
{
	if (typeSolution.contains("perfect")) {
		return getElementsFromApi(mPerfectRepoApi);
	}

	return getElementsFromApi(mOrdinaryRepoApi);
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
