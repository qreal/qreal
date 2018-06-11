/* Copyright 2018 Konstantin Batoev
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

#include <qrrepo/repoApi.h>
#include <qrkernel/ids.h>

#include <generatorBase/semanticTree/semanticNode.h>
#include <generatorBase/semanticTree/semanticTree.h>

namespace generatorBase {
namespace structurizatorNodes {

class IntermediateNode : public QObject
{
	Q_OBJECT

public:

	enum Type {
		simple
		, block
		, ifThenElseCondition
		, switchCondition
		, infiniteloop
		, whileloop
		, breakNode
		, nodeWithBreaks
	};

	explicit IntermediateNode(QObject *parent);

	virtual Type type() const = 0;
	virtual qReal::Id firstId() const = 0;
	virtual bool analyzeBreak() = 0;
	bool hasBreakInside() const;

protected:
	bool mHasBreakInside;
	bool mBreakWasAnalyzed;
};

class SimpleNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit SimpleNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;
	qReal::Id firstId() const;
	bool analyzeBreak();

	qReal::Id id() const;
private:
	const qReal::Id mId;
};

class BreakNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit BreakNode(const qReal::Id &id, QObject *parent);

	Type type() const;
	qReal::Id firstId() const;
	bool analyzeBreak();

private:
	const qReal::Id mId;
};

class IfNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit IfNode(IntermediateNode *condition, IntermediateNode *thenBranch, IntermediateNode *elseBranch
			, IntermediateNode *exit, QObject *parent);

	IntermediateNode *condition() const;
	IntermediateNode *thenBranch() const;
	IntermediateNode *elseBranch() const;
	IntermediateNode *exit() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mCondition;
	IntermediateNode *mThenBranch;
	IntermediateNode *mElseBranch;
	IntermediateNode *mExit;
};


class NodeWithBreaks : public IntermediateNode
{
	Q_OBJECT

public:
	explicit NodeWithBreaks(IntermediateNode *condition, QList<IntermediateNode *> &exitBranches
			, QObject *parent);

	IntermediateNode *condition() const;
	QList<IntermediateNode *> exitBranches() const;
	QList<IntermediateNode *> restBranches() const;

	void setRestBranches(const QList<IntermediateNode *> &restBranches);
	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mCondition;
	QList<IntermediateNode *> mExitBranches;
	QList<IntermediateNode *> mRestBranches;
};

class SwitchNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit SwitchNode(IntermediateNode *condition, const QList<IntermediateNode *> &branches
			, IntermediateNode *exit, QObject *parent);

	IntermediateNode *condition() const;
	QList<IntermediateNode *> branches() const;
	IntermediateNode *exit() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mCondition;
	const QList<IntermediateNode *> mBranches;
	IntermediateNode *mExit;
};


class BlockNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit BlockNode(IntermediateNode *firstNode, IntermediateNode *secondNode, QObject *parent);

	IntermediateNode *firstNode() const;
	IntermediateNode *secondNode() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mFirstNode;
	IntermediateNode *mSecondNode;
};

class WhileNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit WhileNode(IntermediateNode *headNode, IntermediateNode *bodyNode, IntermediateNode *exitNode
			, QObject *parent);

	IntermediateNode *headNode() const;
	IntermediateNode *bodyNode() const;
	IntermediateNode *exitNode() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mHeadNode;
	IntermediateNode *mBodyNode;
	IntermediateNode *mExitNode;
};


class SelfLoopNode : public IntermediateNode
{
	Q_OBJECT

public:
	explicit SelfLoopNode(IntermediateNode *bodyNode, QObject *parent = nullptr);

	IntermediateNode *bodyNode() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mBodyNode;
};

}

}
