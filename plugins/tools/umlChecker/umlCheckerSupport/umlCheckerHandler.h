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

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>

namespace qReal {

class UmlCheckerHandler
{

public:
	UmlCheckerHandler();

	virtual ~UmlCheckerHandler();

	void init(const QString &ordinaryPath, const QString &perfectSolutionPath);
	void initEdgesVariants();
	void clear();
	bool matchingExternalEdges(const QMultiHash<QString, Id> &residue);

	void researchEdge(QMultiHash<QString, Id> &residue, const IdList &values);

	bool matchingResult();
	bool matchingStep(const QList<QPair<QString, QString> > &blockNames);

	void clearAfterMatching();

	bool matchingLinksOfNode(const IdList &perfectLinks, const IdList &ordinaryLinks);

	bool matchNodeElement(const Id &id, IdList &ordinaryValues, const QString &blockName);

	bool checkMatchingNodes(IdList &perfectValues, IdList &ordinaryValues, const QString &blockName);

	bool matchingNodesInsideABlock(QMultiHash<QString, Id> perfectElements
			, QMultiHash<QString, Id> &ordinaryElements, const QString &blockName);

private:
	QMultiHash<QString, Id> getElementsFromApi(qrRepo::RepoApi *repoApi) const;

	QStringList getOptionsForBlock(const QString &blockFile);

	QString mQRealFilesPath;
	QString mPathToPerfect;
	QString mPathToOrdinary;

	QList<QPair<QString, QString>> mBlockNames;
	QStringList mEdges;
	QMultiHash<QString, QPair<QString, QStringList>> mEdgesVariants;
	QStringList mPerfectFileNames;

	qrRepo::RepoApi *mPerfectRepoFromList;
	qrRepo::RepoApi *mOrdinaryRepoApi;
};
}
