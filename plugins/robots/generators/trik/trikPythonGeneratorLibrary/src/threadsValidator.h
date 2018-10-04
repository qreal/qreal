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

#include "generatorBase/robotsDiagramVisitor.h"

typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

namespace generatorBase {

/// Checks a given diagram for inconsistencies of using threads in Qt Script generator
/// (like joining unnamed threads or creating threads in a loop).
class ThreadsValidator : public RobotsDiagramVisitor
{
public:
	ThreadsValidator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, qReal::ErrorReporterInterface &errorReporter);

	bool validate(const qReal::Id &startNode, const QString &threadId);

private:
	void visitRegular(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFinal(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitConditional(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitLoop(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFork(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitJoin(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitUnknown(const qReal::Id &id, const QList<LinkInfo> &links) override;

	void visitGeneral(const qReal::Id &id, const QList<LinkInfo> &links);
	void visitForkFirstStage(const qReal::Id &id, QList<LinkInfo> &links);
	void visitForkSecondStage(const qReal::Id &id, QList<LinkInfo> &links);
	bool achiavable(const qReal::Id &id, const qReal::Id &source);

	void unknownThread(const QList<LinkInfo> &links);
	void error(const QString &message, const qReal::Id &id);
	bool checkForConnected(const LinkInfo &link);

	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	bool mSomethingChanged;
	bool mNoErrors;
	bool mSecondStage;
	qReal::Id mStartNode;
	QString mThreadId;
	QHash<qReal::Id, QString> mBlockThreads;
	QHash<QString, qReal::Id> mThreadCreatingBlocks;
	QSet<qReal::Id> mVisitedBlocks;
};

}
