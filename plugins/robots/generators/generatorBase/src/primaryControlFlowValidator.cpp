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

#include "generatorBase/primaryControlFlowValidator.h"

using namespace generatorBase;
using namespace qReal;

/// @todo: Unify code with interpreter

PrimaryControlFlowValidator::PrimaryControlFlowValidator(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, QObject *parent)
	: QObject(parent)
	, RobotsDiagramVisitor(repo, customizer)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
{
}

bool PrimaryControlFlowValidator::validate(const qReal::Id &diagramId, const QString &threadId)
{
	Q_UNUSED(threadId)

	mIfBranches.clear();
	mLoopBranches.clear();

	mDiagram = diagramId;
	findInitialNode();
	if (mInitialNode.isNull()) {
		error(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"), mDiagram);
		return false;
	}

	mErrorsOccured = false;
	startSearch(mInitialNode);

	return !mErrorsOccured;
}

PrimaryControlFlowValidator *PrimaryControlFlowValidator::clone()
{
	return new PrimaryControlFlowValidator(mRepo, mErrorReporter, mCustomizer, parent());
}

qReal::Id PrimaryControlFlowValidator::initialNode() const
{
	return mInitialNode;
}

QPair<LinkInfo, LinkInfo> PrimaryControlFlowValidator::ifBranchesFor(const qReal::Id &id) const
{
	return mIfBranches[id];
}

QPair<LinkInfo, LinkInfo> PrimaryControlFlowValidator::loopBranchesFor(const qReal::Id &id) const
{
	return mLoopBranches[id];
}

void PrimaryControlFlowValidator::visitRegular(const Id &id
		, const QList<LinkInfo> &links)
{
	if (links.size() != 1) {
		error(QObject::tr("This element must have exactly ONE outgoing link"), id);
	} else {
		checkForConnected(links[0]);
	}
}

void PrimaryControlFlowValidator::visitFinal(const Id &id
		, const QList<LinkInfo> &links)
{
	if (!links.isEmpty()) {
		error(QObject::tr("Final node must not have outgoing links"), id);
	}
}

void PrimaryControlFlowValidator::visitConditional(const Id &id
		, const QList<LinkInfo> &links)
{
	if (links.size() != 2) {
		error(QObject::tr("If block must have exactly TWO outgoing links"), id);
		return;
	}

	// In correct case exactly 2 of this 3 would be non-null
	const LinkInfo *trueLink = nullptr;
	const LinkInfo *falseLink = nullptr;
	const LinkInfo *nonMarkedLink = nullptr;

	for (const LinkInfo &link : links) {
		checkForConnected(link);

		switch (guardOf(link.linkId)) {
		case trueGuard:
			if (trueLink) {
				error(QObject::tr("Two outgoing links marked with 'true' found"), id);
				return;
			} else {
				trueLink = &link;
			}
			break;

		case falseGuard:
			if (falseLink) {
				error(QObject::tr("Two outgoing links marked with 'false' found"), id);
				return;
			} else {
				falseLink = &link;
			}
			break;

		default:
			if (nonMarkedLink) {
				error(QObject::tr("There must be at least one link with \"true\" or \"false\" marker on it"), id);
				return;
			} else {
				nonMarkedLink = &link;
			}
			break;
		}
	}

	// Now we have correctly linked branches. Determining who is who...
	QPair<LinkInfo, LinkInfo> branches;
	if (!trueLink) {
		branches.first = *nonMarkedLink;
		branches.second = *falseLink;
	} else if (!falseLink) {
		branches.first = *trueLink;
		branches.second = *nonMarkedLink;
	} else {
		branches.first = *trueLink;
		branches.second = *falseLink;
	}

	mIfBranches[id] = branches;
}

void PrimaryControlFlowValidator::visitLoop(const Id &id
		, const QList<LinkInfo> &links)
{
	if (links.size() != 2) {
		error(QObject::tr("Loop block must have exactly TWO outgoing links"), id);
		return;
	}

	// In correct case must be non-null and different
	const LinkInfo *bodyLink = nullptr;
	const LinkInfo *nonMarkedBlock = nullptr;

	for (const LinkInfo &link : links) {
		checkForConnected(link);

		switch (guardOf(link.linkId)) {
		case bodyGuard:
			if (bodyLink) {
				error(QObject::tr("Two outgoing links marked with \"body\" found"), id);
				return;
			} else {
				bodyLink = &link;
			}
			break;
		default:
			if (nonMarkedBlock) {
				error(QObject::tr("There must be a link with \"body\" marker on it"), id);
				return;
			} else {
				nonMarkedBlock = &link;
			}
			break;
		}
	}

	if (bodyLink->target == nonMarkedBlock->target) {
		error(QObject::tr("Outgoing links from loop block must be connected to different blocks"), id);
	}

	mLoopBranches[id] = qMakePair(*bodyLink, *nonMarkedBlock);
}

void PrimaryControlFlowValidator::visitSwitch(const Id &id
		, const QList<LinkInfo> &links)
{
	QSet<QString> branches;
	bool defaultBranchFound = false;

	if (links.size() < 2) {
		error(QObject::tr("There must be at list TWO links outgoing from switch block"), id);
		return;
	}

	for (const LinkInfo &link : links) {
		checkForConnected(link);

		const QString condition = mRepo.property(link.linkId, "Guard").toString();
		if (condition.isEmpty()) {
			if (defaultBranchFound) {
				error(QObject::tr("There must be exactly one link without marker on it (default branch)"), id);
				return;
			} else {
				defaultBranchFound = true;
			}
		} else {
			if (branches.contains(condition)) {
				error(QObject::tr("Duplicate case branch: '%1'").arg(condition), link.linkId);
				return;
			}

			branches << condition;
		}
	}

	if (!defaultBranchFound) {
		error(QObject::tr("There must be a link without marker on it (default branch)"), id);
	}
}

void PrimaryControlFlowValidator::visitUnknown(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)
	error(QObject::tr("Unknown block type"), id);
}

void PrimaryControlFlowValidator::visitFork(const Id &id, QList<LinkInfo> &links)
{
	if (links.size() < 2) {
		error(QObject::tr("Fork block must have at least TWO outgoing links"), id);
		return;
	}

	for (const LinkInfo &link : links) {
		checkForConnected(link);
	}
}

void PrimaryControlFlowValidator::visitJoin(const Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void PrimaryControlFlowValidator::error(const QString &message, const qReal::Id &id)
{
	mErrorReporter.addError(message, id);

	// Returns false for possibility of one-line 'return error(...);'
	mErrorsOccured = true;
}

bool PrimaryControlFlowValidator::checkForConnected(const LinkInfo &link)
{
	if (!link.connected) {
		error(QObject::tr("Outgoing link is not connected"), link.linkId);
		return false;
	}

	return true;
}

void PrimaryControlFlowValidator::findInitialNode()
{
	const qReal::IdList diagramNodes(mRepo.children(mDiagram));
	for (const qReal::Id &diagramNode : diagramNodes) {
		if (mCustomizer.isInitialNode(diagramNode)) {
			mInitialNode = mRepo.logicalId(diagramNode);
			return;
		}
	}

	mInitialNode = Id();
}
