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

UmlCheckerHandler::UmlCheckerHandler()
{
	mQRealSourceFilesPath = "/home/julia/qreal/qreal";
	mPathToOrdinary = mQRealSourceFilesPath + "/plugins/tools/umlChecker/ordinary/";

	mOrdinaryRepoApi = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/ordinary", true);
}

UmlCheckerHandler::~UmlCheckerHandler()
{
}

void UmlCheckerHandler::init(const QString &ordinaryPath, const QString &perfectSolutionPath)
{
	mPathToPerfect = perfectSolutionPath;
	mPerfectRepoFromList = new qrRepo::RepoApi(mQRealSourceFilesPath + "/plugins/umlChecker/perfect/", true);
	QDir dir(mPathToPerfect);
	QStringList filters;
	filters << "*.txt";
	dir.setNameFilters(filters);
	const QStringList blockNames = dir.entryList();
	for (const QString &fileName : blockNames) {
		QPair<QString, QString> pair = qMakePair(dir.absoluteFilePath(fileName), fileName);
		mBlockNames.append(pair);
	}


	QString perfectEdgesPath = perfectSolutionPath + "edges/";
	QDir edgesDir(perfectEdgesPath);
	edgesDir.setNameFilters(filters);
	const QStringList edgeNames = edgesDir.entryList();
	for (const QString &fileName : edgeNames) {
		mEdges.append(edgesDir.absoluteFilePath(fileName));
	}

	mPathToOrdinary = ordinaryPath;
	mOrdinaryRepoApi->open(ordinaryPath);
	initEdgesVariants();
}


void UmlCheckerHandler::initEdgesVariants()
{
	for (const QString &fileName : mEdges) {
		QStringList result;
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly)) {

			QTextStream in(&file);
			const QString from = in.readLine();
			const QString to = in.readLine();

			while(!in.atEnd()) {
				QString line = in.readLine();
				result.append(line);
			}

			const QString key = from + to;
			const QPair<QString, QStringList> pair = qMakePair(fileName, result);
			mEdgesVariants.insertMulti(key, pair);
		}

		file.close();
	}
}

void UmlCheckerHandler::clear()
{
	mBlockNames.clear();
	delete mPerfectRepoFromList;
}

QStringList UmlCheckerHandler::getOptionsForBlock(const QString &blockFile)
{
	QStringList result;

	QFile file(blockFile);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while(!in.atEnd()) {
			QString line = in.readLine();
			result.append(line);
		}
	}

	file.close();
	return result;
}


bool UmlCheckerHandler::matchingLinksOfNode(const IdList &perfectLinks, const IdList &ordinaryLinks)
{
	IdList changeblePerfectLinks;
	for (const Id &perfectLink : perfectLinks) {
		for (const Id &ordinaryLink : ordinaryLinks) {
			if (perfectLink.element() == ordinaryLink.element() && !changeblePerfectLinks.contains(perfectLink)) {
				changeblePerfectLinks.append(perfectLink);
			}
		}
	}


	return changeblePerfectLinks.count() == perfectLinks.count();
}


bool UmlCheckerHandler::matchNodeElement(const Id &id, IdList &ordinaryValues, const QString &blockName)
{
	const IdList incoming = mPerfectRepoFromList->incomingLinks(id);
	const IdList outgoing = mPerfectRepoFromList->outgoingLinks(id);

	for (const Id &ordinaryId : ordinaryValues) {
		const IdList incomingOrdinary = mOrdinaryRepoApi->incomingLinks(ordinaryId);
		const IdList outgoingOrdinary = mOrdinaryRepoApi->outgoingLinks(ordinaryId);
		bool incomingMatch = matchingLinksOfNode(incoming, incomingOrdinary);
		bool outgoingMatch = matchingLinksOfNode(outgoing, outgoingOrdinary);
		if (incomingMatch && outgoingMatch) {
			mOrdinaryRepoApi->setProperty(ordinaryId, "blockName", QVariant(blockName));
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

bool UmlCheckerHandler::checkMatchingNodes(IdList &perfectValues, IdList &ordinaryValues, const QString &blockName)
{
	IdList changeablePerfect = perfectValues;
	IdList changeableOrdinary = ordinaryValues;

	IdList lastShiftIds = changeablePerfect;

	for (int i = 0; i < perfectValues.size(); ++i) {
		for (const Id &id : changeablePerfect) {
			bool nodeMatch = matchNodeElement(id, changeableOrdinary, blockName);
			if (!nodeMatch) {
				changeablePerfect = doShift(lastShiftIds);
				break;
			}

			changeablePerfect.removeOne(id);
		}
	}

	ordinaryValues = changeableOrdinary;
	return changeablePerfect.count() == 0;
}

void UmlCheckerHandler::researchEdge(QMultiHash<QString, Id> &residue, const IdList &values)
{
	for (const Id &id : values) {
		Id from = mOrdinaryRepoApi->from(id);
		Id to = mOrdinaryRepoApi->to(id);

		if (mOrdinaryRepoApi->hasProperty(from, "blockName") && mOrdinaryRepoApi->hasProperty(to, "blockName")) {
			if (mOrdinaryRepoApi->property(from, "blockName") == mOrdinaryRepoApi->property(to, "blockName")) {
				mOrdinaryRepoApi->setProperty(id, "blockName", QVariant(mOrdinaryRepoApi->property(to, "blockName")));
			} else {
				const QString fromBlock = mOrdinaryRepoApi->property(from, "blockName").toString();
				const QString toBlock = mOrdinaryRepoApi->property(to, "blockName").toString();
				const QString key = fromBlock + toBlock;
				residue.insertMulti(key, id);
			}
		}
	}
}


bool UmlCheckerHandler::matchingNodesInsideABlock(QMultiHash<QString, Id> perfectElements
		, QMultiHash<QString, Id> &ordinaryElements, const QString &blockName)
{
	const QString umlClass = "UmlClass";
	IdList ordinaryValues = ordinaryElements.values(umlClass);
	IdList perfectValues = perfectElements.values(umlClass);
	bool resOfCheckMatching = checkMatchingNodes(perfectValues, ordinaryValues, blockName);
	if (resOfCheckMatching == false) {
		return resOfCheckMatching;
	}

	ordinaryElements.remove(umlClass);

	for (const Id &ordValue : ordinaryValues) {
		ordinaryElements.insertMulti(umlClass, ordValue);
	}

	return true;
}

bool UmlCheckerHandler::matchingResult()
{
	QList<QPair<QString, QString>> blockNames = mBlockNames;
	for (int i = 0; i < blockNames.size(); ++i) {
		bool matchingRes = matchingStep(blockNames);
		if (matchingRes) {
			return true;
		} else {
			blockNames.move(0, blockNames.size() - 1);
			clearAfterMatching();
		}
	}

	return false;
}

bool UmlCheckerHandler::matchingStep(const QList<QPair<QString, QString>> &blockNames)
{
	QMultiHash<QString, Id> ordinaryElements = getElementsFromApi(mOrdinaryRepoApi);
	QMultiHash<QString, Id> intermediateOrdElems = ordinaryElements;

	for (int i = 0; i < blockNames.size(); ++i) {
		QPair<QString, QString> blockName = blockNames.value(i);
		QStringList blocksList = getOptionsForBlock(blockName.first);
		bool matchingBlock = false;

		for (const QString &fileName : blocksList) {
			mPerfectRepoFromList->open(fileName);
			const QMultiHash<QString, Id> perfectElements = getElementsFromApi(mPerfectRepoFromList);

			ordinaryElements = intermediateOrdElems;
			if (blockName.second.contains(".txt")) {
				blockName.second.chop(4);
			}

			matchingBlock = matchingNodesInsideABlock(perfectElements, ordinaryElements, blockName.second);
			if (matchingBlock) {
				break;
			}
		}

		if (matchingBlock) {
			intermediateOrdElems = ordinaryElements;
		} else {
			return false;
		}
	}

	QMultiHash<QString, Id> residue;

	for (const QString &edgeKey : ordinaryElements.uniqueKeys()) {
		const IdList edgeValues = ordinaryElements.values(edgeKey);
		researchEdge(residue, edgeValues);
		ordinaryElements.remove(edgeKey);
	}

	bool externalEquals = residue.size() == mEdgesVariants.size();

	bool matchExternalEdges = matchingExternalEdges(residue);
	return ordinaryElements.size() == 0 && matchExternalEdges && externalEquals;
}


bool UmlCheckerHandler::matchingExternalEdges(const QMultiHash<QString, Id> &residue)
{
	QMultiHash<QString, Id> residueChangeable = residue;

	for (const QString &key : residueChangeable.uniqueKeys()) {
		const IdList ids = residueChangeable.values(key);
		for (const Id &id : ids) {
			const QString idType = id.element();
			const QList<QPair<QString, QStringList>> edges = mEdgesVariants.values(key);
			for (const QPair<QString, QStringList> &edgeSet : edges) {
				for (const QString &type : edgeSet.second) {
					if (type.contains(idType) && residueChangeable.contains(key, id)) {
						residueChangeable.remove(key, id);
						break;
					}
				}

			}
		}
	}

	return residueChangeable.size() == 0;
}

void UmlCheckerHandler::clearAfterMatching()
{
	mOrdinaryRepoApi->open(mPathToOrdinary);
}

QMultiHash<QString, Id> UmlCheckerHandler::getElementsFromApi(qrRepo::RepoApi *repoApi) const
{
	QMultiHash<QString, Id> hashElements;
	const IdList elements = repoApi->graphicalElements();
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
