#include "structurizator.h"

using namespace generatorBase;

utils::SimpleNode::SimpleNode(const qReal::Id &id, QObject *parent)
	: Node(parent)
	, mId(id)
{
	mIdsInvolved = { id };
}

qReal::Id generatorBase::utils::SimpleNode::id() const
{
	return mId;
}

utils::IfNode::IfNode(utils::Node *condition
							, generatorBase::utils::Node *thenBranch
							, generatorBase::utils::Node *elseBranch
							, QObject *parent)
	: Node(parent)
	, mCondition(condition)
	, mThenBranch(thenBranch)
	, mElseBranch(elseBranch)
	, mIsIfThenForm(elseBranch == nullptr)
{
	mIdsInvolved = condition->ids() + thenBranch->ids() + elseBranch->ids();
}

utils::Node *utils::IfNode::condition() const
{
	return mCondition;
}

utils::Node *utils::IfNode::thenBranch() const
{
	return mThenBranch;
}

utils::Node *utils::IfNode::elseBranch() const
{
	return mElseBranch;
}

utils::Node::Node(QObject *parent)
	: QObject(parent)
	, mIdsInvolved(QSet<qReal::Id>())
{
}

utils::Node::Node(QSet<qReal::Id> &ids, QObject *parent)
	: QObject(parent)
	, mIdsInvolved(ids)
{
}

QSet<qReal::Id> utils::Node::ids() const
{
	return mIdsInvolved;
}

utils::SwitchNode::SwitchNode(utils::Node *condition, const QList<Node *> &branches, QObject *parent)
	: Node(parent)
	, mCondition(condition)
	, mBranches(QList<Node *>(branches))
{
	mIdsInvolved = {};
	for (const utils::Node * node : branches) {
		mIdsInvolved.unite(node->ids());
	}
}

utils::Node *utils::SwitchNode::condition() const
{
	return mCondition;
}

QList<utils::Node *> utils::SwitchNode::branches() const
{
	return mBranches;
}

utils::BlockNode::BlockNode(utils::Node *firstNode, utils::Node *secondNode, QObject *parent)
	: Node(parent)
	, mFirstNode(firstNode)
	, mSecondNode(secondNode)
{
	mIdsInvolved = firstNode->ids() + secondNode->ids();
}

utils::Node *utils::BlockNode::firstNode() const
{
	return mFirstNode;
}

utils::Node *utils::BlockNode::secondNode() const
{
	return mSecondNode;
}

utils::WhileNode::WhileNode(utils::Node *headNode, utils::Node *bodyNode, QObject *parent)
	: Node(parent)
	, mHeadNode(headNode)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = mHeadNode->ids() + mBodyNode->ids();
}

utils::SelfLoopNode::SelfLoopNode(utils::Node *bodyNode, QObject *parent)
	: Node(parent)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = bodyNode->ids();
}

Structurizator::Structurizator(const qrRepo::RepoApi &repo, const qReal::IdList &vertecesIds, QObject *parent)
	: QObject(parent)
	, mRepo(repo)
	, initialIds(vertecesIds)
	, mVertecesNumber(1)
	, mEdgesNumber(1)
	, mStartVertex(-1)
	, mMaxPostOrderTime(-1)
{
	// remembering ids and verteces
	for (const qReal::Id &id : vertecesIds) {
		mMapIdToInt[id] = mVertecesNumber;
		mVerteces.insert(mVertecesNumber);
		mVertecesNumber++;
	}

	findStartVertex();

	// constructing graph
	createGraph();

	// post order
	calculatePostOrder();

	calculateDominators();

	createInitialNodesForIds();
}

utils::Node *Structurizator::performStructurization()
{

	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		int t = 0;
		while (t <= mMaxPostOrderTime && mVerteces.size() > 1) {

//			if (!checkAllStructures()) {
//				bool flag = checkAllStructures();
//			}
//			Q_ASSERT(checkAllStructures());
			int v = mPostOrder.key(t);
			if (!v) {
				qDebug() << "Problem! time = " << t;
			}

			QSet<int> edgesToRemove = {};
			QSet<int> verteces;
			if (isBlock(v, edgesToRemove, verteces)) {
				reduceBlock(v, edgesToRemove, verteces);
				qDebug() << "Block";
			} else if (isDummySwitch(v, edgesToRemove, verteces)) {
				reduceDummySwitch(v, edgesToRemove, verteces);
				qDebug() << "Dummy switch";
			} else if (isSwitch(v, edgesToRemove, verteces)) {
				reduceSwitch(v, edgesToRemove, verteces);
				qDebug() << "Switch";
			} else if (isIfThenElse(v, edgesToRemove, verteces)) {
				reduceIfThenElse(v, edgesToRemove, verteces);
				qDebug() << "If then else";
			} else if (isIfThen(v, edgesToRemove, verteces)) {
				reduceIfThen(v, edgesToRemove, verteces);
				qDebug() << "If then";
			} else if (isInfiniteLoop(v, edgesToRemove, verteces)) {
				reduceInfiniteLoop(v, edgesToRemove, verteces);
				qDebug() << "Infinite loop";
			} else if (isWhileLoop(v, edgesToRemove, verteces)) {
				reduceWhileLoop(v, edgesToRemove, verteces);
				qDebug() << "While loop";

			} else {
//				QSet<int> reachUnder;
//				obtainReachUnder(v, reachUnder);
//				bool ok = dealWithReachUnder(v, reachUnder);
//				if (ok) {
//					t = 0;
//					mSomethingChanged = true;
//				} else {
//					t++;
//				}

//				continue;
			}

			if (verteces.size()) {
				t -= (verteces.size() - 1);
			}
			somethingChanged = true;
		}
	}

}

void Structurizator::reduceBlock(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::reduceIfThenElse(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::reduceIfThen(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::reduceSwitch(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::reduceDummySwitch(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::reduceInfiniteLoop(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::reduceWhileLoop(int v, QSet<int> &edgesToRemove, QSet<int> &verteces)
{

}

void Structurizator::createGraph()
{
	for (const qReal::Id &vertex : initialIds) {
		for (const qReal::Id &link : mRepo.outgoingLinks(vertex)) {
			int v = mMapIdToInt[vertex];
			int u = mMapIdToInt[mRepo.otherEntityFromLink(link, vertex)];

			if (!mFollowers[v].contains(u)) {
				mFollowers[v].insert(u);
				mPredecessors[u].insert(v);

				mEdges.insert(mEdgesNumber);
				mMapEdgeNumberToVerteces[mEdgesNumber] = QPair<int, int>(v, u);
				mEdgesNumber++;
			}
		}
	}
}

void Structurizator::calculateDominators()
{
	for (const int u : mVerteces) {
		mDominators[u] = mVerteces;
	}

	mStartVertex = { mStartVertex };

	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		for (const int v : mVerteces) {
			if (v == mStartVertex)
				continue;

			QSet<int> doms = mVerteces;
			for (const int u : mPredecessors[v]) {
				doms = doms.intersect(mDominators[u]);
			}

			doms.insert(v);

			if (doms != mDominators[v]) {
				mDominators[v] = doms;
				somethingChanged = true;
			}
		}
	}


}

void Structurizator::findStartVertex()
{
	for (const int u : mVerteces) {
		if (mPredecessors[u].isEmpty()) {
			mStartVertex = u;
		}
	}
}

void Structurizator::calculatePostOrder()
{
	QMap<int, bool> used;
	for (const int v : mVerteces) {
		used[v] = false;
	}

	int currentTime = 0;
	dfs(mStartVertex, currentTime, used);

	mMaxPostOrderTime = currentTime - 1;
}

void Structurizator::createInitialNodesForIds()
{
	for (const int v : mVerteces) {
		mTrees[v] = new utils::SimpleNode(mMapIdToInt.key(v), this);
	}
}

void Structurizator::dfs(int v, int currentTime, QMap<int, bool> &used)
{
	used[v] = true;
	for (const int u : mPredecessors[v]) {
		if (!used[u]) {
			dfs(u, currentTime, used);
		}
	}

	mPostOrder[v] = currentTime;
	currentTime++;
}
