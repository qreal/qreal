#include "structurizator.h"

using namespace generatorBase;
using namespace utils;

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
			QMap<QString, int> vertecesRoles;
			if (isBlock(v, edgesToRemove, vertecesRoles)) {
				reduceBlock(edgesToRemove, vertecesRoles);
				qDebug() << "Block";
			} else if (isSwitch(v, edgesToRemove, vertecesRoles)) {
				reduceSwitch(edgesToRemove, vertecesRoles);
				qDebug() << "Switch";
			} else if (isIfThenElse(v, edgesToRemove, vertecesRoles)) {
				reduceIfThenElse(edgesToRemove, vertecesRoles);
				qDebug() << "If then else";
			} else if (isIfThen(v, edgesToRemove, vertecesRoles)) {
				reduceIfThen(edgesToRemove, vertecesRoles);
				qDebug() << "If then";
			} else if (isInfiniteLoop(v, edgesToRemove, vertecesRoles)) {
				reduceInfiniteLoop(edgesToRemove, vertecesRoles);
				qDebug() << "Infinite loop";
			} else if (isWhileLoop(v, edgesToRemove, vertecesRoles)) {
				reduceWhileLoop(edgesToRemove, vertecesRoles);
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

				continue;
			}

			if (vertecesRoles.size()) {
				t -= (vertecesRoles.size() - 1);
			}
			somethingChanged = true;
		}
	}

	if (mTrees.size() == 1) {
		return mTrees[mStartVertex];
	}

	return nullptr;
}

bool Structurizator::isBlock(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (outgoingEdgesNumber(v) != 1) {
		return false;
	}

	int u = mFollowers[v].first();
	if (outgoingEdgesNumber(u) <= 1 && incomingEdgesNumber(u) == 1) {
		vertecesRoles["block1"] = v;
		vertecesRoles["block2"] = u;

		edgesToRemove = { mMapEdgeNumberToVerteces[QPair<int, int>(v, u)] };
		return true;
	}
}

bool Structurizator::isIfThenElse(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (outgoingEdgesNumber(v) != 2) {
		return false;
	}

	int u1 = mFollowers[v].first();
	int u2 = mFollowers[v].last();
	if (incomingEdgesNumber(u1) != 1 || incomingEdgesNumber(u2) != 1) {
		return false;
	}

	if ((outgoingEdgesNumber(u1) == 0 && outgoingEdgesNumber(u2) == 0) ||
				(outgoingEdgesNumber(u1) == 1 && outgoingEdgesNumber(u2) == 1 &&
				mFollowers[u1].first() == mFollowers[u2].first())) {

		vertecesRoles["condition"] = v;
		vertecesRoles["then"] = u1;
		vertecesRoles["else"] = u2;
		QPair<int, int> p1 = QPair<int, int>(v, u1);
		QPair<int, int> p2 = QPair<int, int>(v, u2);
		edgesToRemove += {mMapEdgeNumberToVerteces[p1], mMapEdgeNumberToVerteces[p2]};
		return true;
	}

	return false;
}

bool Structurizator::isIfThen(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (outgoingEdgesNumber(v) != 2) {
		return false;
	}

	int u1 = mFollowers[v].first();
	int u2 = mFollowers[v].last();

	int thenNumber = -1;
	int elseNumber = -1;
	if (checkIfThenHelper(u1, u2)) {
		thenNumber = u1;
		elseNumber = u2;
	} else if (checkIfThenHelper(u2, u1)) {
		thenNumber = u2;
		elseNumber = u1;
	}

	if (thenNumber == -1) {
		return false;
	}

	vertecesRoles["condition"] = v;
	vertecesRoles["then"] = thenNumber;
	vertecesRoles["else"] = elseNumber;

	edgesToRemove = {mMapEdgeNumberToVerteces[QPair<int, int>(v, u1)],
					 mMapEdgeNumberToVerteces[QPair<int, int>(v, u2)]};

	return true;
}

bool Structurizator::isSwitch(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (mFollowers[v].size() < 2) {
		return false;
	}

	int exit = -1;
	QSet<int> verteces = {};
	QSet<int> edges = {};
	for (const int u : mFollowers[v]) {
		if (incomingEdgesNumber(u) != 1 || outgoingEdgesNumber(u) >= 2) {
			return false;
		}

		if (outgoingEdgesNumber(u) == 1) {
			int m = mFollowers[u].first();
			if (exit == -1) {
				exit = m;
			} else if (m != exit) {
				return false;
			}
		}

		verteces.insert(u);
		edges.insert(mMapEdgeNumberToVerteces[QPair<int, int>(v, u)]);
	}

	vertecesRoles["head"] = v;
	edgesToRemove = edges;

	int cnt = 1;
	for (int u : verteces) {
		vertecesRoles[QString::number(cnt)] = u;
		cnt++;
	}

	return true;
}

bool Structurizator::isInfiniteLoop(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (outgoingEdgesNumber(v) != 1) {
		return false;
	}

	int u = mFollowers[v].first();
	if (u != v) {
		return false;
	}

	vertecesRoles["body"] = v;
	edgesToRemove.insert(mMapEdgeNumberToVerteces[QPair<int, int>(v, v)]);
	return true;
}

bool Structurizator::isWhileLoop(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (outgoingEdgesNumber(v) != 2) {
		return false;
	}

	int u1 = mFollowers[v].first();
	int u2 = mFollowers[v].last();

	int bodyNumber = -1;
	if (checkWhileLoopHelper(v, u1)) {
		bodyNumber = u1;
	} else if (checkWhileLoopHelper(v, u2)) {
		bodyNumber = u2;
	}

	if (bodyNumber == -1) {
		return false;
	}

	edgesToRemove = { mMapEdgeNumberToVerteces[QPair<int, int>(v, bodyNumber)],
							mMapEdgeNumberToVerteces[QPair<int, int>(bodyNumber, v)] };

	vertecesRoles["head"] = v;
	vertecesRoles["body"] = bodyNumber;

	return true;
}

bool Structurizator::checkIfThenHelper(int thenNumber, int exitNumber)
{
	if (incomingEdgesNumber(thenNumber) == 1 && outgoingEdgesNumber(thenNumber) == 1) {
		if (mFollowers[thenNumber].contains(exitNumber)) {
			return true;
		}
	}

	return false;
}

bool Structurizator::checkWhileLoopHelper(int head, int body)
{
	if (incomingEdgesNumber(body) == 1 && outgoingEdgesNumber(body) == 1) {
		int w = mFollowers[body].first();
		if (w == head) {
			return true;
		}
	}

	return false;
}

void Structurizator::reduceBlock(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	BlockNode *block = new BlockNode(mTrees[vertecesRoles["block1"]], mTrees[vertecesRoles["block2"]], this);

	appendVertex(block, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceIfThenElse(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	utils::IfNode *ifNode = new IfNode(mTrees[vertecesRoles["condition"]], mTrees[vertecesRoles["then"]], mTrees[vertecesRoles["else"]], this);

	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceIfThen(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	utils::IfNode *ifNode = new IfNode(mTrees[vertecesRoles["condition"]], mTrees[vertecesRoles["then"]], nullptr, this);

	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceSwitch(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["head"];
	QSet<int> otherVerteces = vertecesRoles.values().toSet();
	otherVerteces.remove(v);

	QList<Node *> branches;
	for (const int u : otherVerteces) {
		branches.append(mTrees[u]);
	}

	utils::SwitchNode *switchNode = new SwitchNode(mTrees[v], branches);

	appendVertex(switchNode, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceInfiniteLoop(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	SelfLoopNode *loopNode = new SelfLoopNode(mTrees[vertecesRoles["body"]], this);

	appendVertex(loopNode, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceWhileLoop(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	WhileNode *whileNode = new WhileNode(mTrees[vertecesRoles["head"]], mTrees[vertecesRoles["body"]], this);

	appendVertex(whileNode, edgesToRemove, vertecesRoles);
}

void Structurizator::replace(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces)
{
	updateEdges(newNodeNumber, edgesToRemove, verteces);
	updatePostOrder(newNodeNumber, verteces);
	updateDominators(newNodeNumber, verteces);
	updateVerteces(newNodeNumber, verteces);
}

void Structurizator::updateEdges(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces)
{
	QMap<int, QVector<int> > followers = mFollowers;

	for (int v : mVerteces) {
		for (int u : followers[v]) {

			QPair<int, int> p = QPair<int, int>(v, u);
			int edgeNumber = mMapEdgeNumberToVerteces[p];

			if (edgesToRemove.contains(edgeNumber)) {
				mEdges.remove(mMapEdgeNumberToVerteces[p]);
				mMapEdgeNumberToVerteces.remove(p);
				continue;
			}

			int newV = v;
			int newU = u;

			if (verteces.contains(v)) {
				newV = newNodeNumber;
			}

			if (verteces.contains(u)) {
				newU = newNodeNumber;
			}

			if (newU == newNodeNumber || newV == newNodeNumber) {
				// removing old information
				mFollowers[v].remove(u);
				mPredecessors[u].remove(v);

				// inserting new information
				if (!mFollowers[newV].contains(newU)) {
					mFollowers[newV].push_back(newU);
					mPredecessors[newU].push_back(newV);

					// inserting edge
					mMapEdgeNumberToVerteces[QPair<int, int>(newV, newU)] = mMapEdgeNumberToVerteces[p];
				}

				// removing old edge
				mMapEdgeNumberToVerteces.remove(p);
			}

		}
	}

	// removing old information
	for (int v : verteces) {
		mFollowers.remove(v);
		mPredecessors.remove(v);
	}
}

void Structurizator::updatePostOrder(int newNodeNumber, QSet<int> &verteces)
{
	int minimum = -1;
	for (int v : verteces) {
		if (minimum == -1 || minimum > mPostOrder[v]) {
			minimum = mPostOrder[v];
		}
	}

	mPostOrder[newNodeNumber] = minimum;

	for (int v : verteces) {
		mPostOrder.remove(v);
	}

	mMaxPostOrderTime = mMaxPostOrderTime - verteces.size() + 1;

	QVector<int> times = mPostOrder.values().toVector();
	std::sort(times.begin(), times.end());

	for (int i = 0; i <= mMaxPostOrderTime; i++) {
		int v = mPostOrder.key(times[i]);
		mPostOrder[v] = i;
	}
}

void Structurizator::updateDominators(int newNodeNumber, QSet<int> &verteces)
{
	// others
	for (int v : mPostOrder.keys()) {
		if (mDominators[v].intersects(verteces)) {
			mDominators[v].subtract(verteces);
			mDominators[v].insert(newNodeNumber);
		}
	}

	// new
	QSet<int> doms = mVerteces;
	for (int v : verteces) {
		doms.intersect(mDominators[v]);
	}

	doms.subtract(verteces);
	doms.insert(newNodeNumber);

	mDominators[newNodeNumber] = doms;

	// old
	for (int v : verteces) {
		mDominators.remove(v);
	}
}

void Structurizator::updateVerteces(int newNodeNumber, QSet<int> &verteces)
{
	mStartVertex = mVerteces.contains(mStartVertex) ? newNodeNumber : mStartVertex;
	mVerteces.subtract(verteces);
	mVerteces.insert(newNodeNumber);
}

void Structurizator::createGraph()
{
	for (const qReal::Id &vertex : initialIds) {
		for (const qReal::Id &link : mRepo.outgoingLinks(vertex)) {
			int v = mMapIdToInt[vertex];
			int u = mMapIdToInt[mRepo.otherEntityFromLink(link, vertex)];

			if (!mFollowers[v].contains(u)) {
				mFollowers[v].push_back(u);
				mPredecessors[u].push_back(v);

				mEdges.insert(mEdgesNumber);
				mMapEdgeNumberToVerteces[QPair<int, int>(v, u)] = mEdgesNumber;
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

void Structurizator::appendVertex(Node *node, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	mTrees[mVertecesNumber] = node;
	mVerteces.insert(mVertecesNumber);

	QSet<int> verteces = vertecesRoles.values().toSet();
	replace(mVertecesNumber, edgesToRemove, verteces);
	mVertecesNumber++;
}

int Structurizator::outgoingEdgesNumber(int v) const
{
	return mFollowers[v].size();
}

int Structurizator::incomingEdgesNumber(int v) const
{
	return mPredecessors[v].size();
}
