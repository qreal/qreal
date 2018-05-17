#include "structurizator.h"

#include <QQueue>

using namespace generatorBase;
using namespace myUtils;

SimpleNode::SimpleNode(const qReal::Id &id, QObject *parent)
	: Node(parent)
	, mId(id)
{
	mIdsInvolved = { id };
}

Node::Type SimpleNode::type() const
{
	return Type::simple;
}

qReal::Id SimpleNode::id() const
{
	return mId;
}

IfNode::IfNode(Node *condition
							, Node *thenBranch
							, Node *elseBranch
							, QObject *parent)
	: Node(parent)
	, mCondition(condition)
	, mThenBranch(thenBranch)
	, mElseBranch(elseBranch)
	, mIsIfThenForm(elseBranch == nullptr)
{
	mIdsInvolved = mCondition->ids() + mThenBranch->ids();

	if (mElseBranch) {
		mIdsInvolved += mElseBranch->ids();
	}
}

Node *IfNode::condition() const
{
	return mCondition;
}

Node *IfNode::thenBranch() const
{
	return mThenBranch;
}

Node *IfNode::elseBranch() const
{
	return mElseBranch;
}

Node::Type IfNode::type() const
{
	return Type::ifThenElseCondition;
}

Node::Node(QObject *parent)
	: QObject(parent)
	, mIdsInvolved(QSet<qReal::Id>())
{
}

Node::Node(QSet<qReal::Id> &ids, QObject *parent)
	: QObject(parent)
	, mIdsInvolved(ids)
{
}

QSet<qReal::Id> Node::ids() const
{
	return mIdsInvolved;
}

SwitchNode::SwitchNode(Node *condition, const QList<Node *> &branches, QObject *parent)
	: Node(parent)
	, mCondition(condition)
	, mBranches(QList<Node *>(branches))
{
	mIdsInvolved = {};
	for (const Node * node : branches) {
		mIdsInvolved.unite(node->ids());
	}
}

Node *SwitchNode::condition() const
{
	return mCondition;
}

QList<Node *> SwitchNode::branches() const
{
	return mBranches;
}

Node::Type SwitchNode::type() const
{
	return Type::switchCondition;
}

BlockNode::BlockNode(Node *firstNode, Node *secondNode, QObject *parent)
	: Node(parent)
	, mFirstNode(firstNode)
	, mSecondNode(secondNode)
{
	mIdsInvolved = firstNode->ids() + secondNode->ids();
}

Node *BlockNode::firstNode() const
{
	return mFirstNode;
}

Node *BlockNode::secondNode() const
{
	return mSecondNode;
}

Node::Type BlockNode::type() const
{
	return Type::block;
}

WhileNode::WhileNode(Node *headNode, Node *bodyNode, QObject *parent)
	: Node(parent)
	, mHeadNode(headNode)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = mHeadNode->ids() + mBodyNode->ids();
}

Node::Type WhileNode::type() const
{
	return Type::whileloop;
}

SelfLoopNode::SelfLoopNode(Node *bodyNode, QObject *parent)
	: Node(parent)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = bodyNode->ids();
}

Node::Type SelfLoopNode::type() const
{
	return Type::infiniteloop;
}

IfWithBreakNode::IfWithBreakNode(Node *condition, Node *actionsBeforeBreak, QObject *parent)
	: Node(parent)
	, mCondition(condition)
	, mActionsBeforeBreak(actionsBeforeBreak)
{
	mIdsInvolved = mCondition->ids();
	if (mActionsBeforeBreak) {
		mIdsInvolved += mActionsBeforeBreak->ids();
	}
}

Node *IfWithBreakNode::condition() const
{
	return mCondition;
}

Node *IfWithBreakNode::actionsBeforeBreak() const
{
	return mActionsBeforeBreak;
}

Structurizator::Structurizator(const qrRepo::RepoApi &repo, QSet<qReal::Id> &vertecesIds, QObject *parent)
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

	// constructing graph
	createGraph();

	findStartVertex();

	// post order
	calculatePostOrder();

	calculateDominators();

	createInitialNodesForIds();
}

Node *Structurizator::performStructurization()
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

			QSet<int> reachUnder;
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

			} else if (isHeadOfCycle(v, reachUnder)) {

				QMap<int, int> nodesWithExits;
				int commonExit = -1;
				bool isCycle = isCycleWithBreaks(reachUnder, nodesWithExits, commonExit);
				if (!isCycle) {
					t++;
					continue;
				}


				reduceConditionsWithBreaks(v, nodesWithExits, commonExit);
			}

			if (vertecesRoles.size()) {
				t -= (vertecesRoles.size() - 1);
				somethingChanged = true;
			} else {
				t++;
			}
		}
	}

	if (mVerteces.size() == 1) {
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

bool Structurizator::isCycleWithBreaks(QSet<int> &reachUnder, QMap<int, int> &nodesWithExits, int &commonExit)
{
	bool result = findCommonExit(reachUnder, nodesWithExits, commonExit);
	if (!result) {
		return false;
	}

	return checkCommonExit(commonExit, nodesWithExits);

	/*


	int oneSavedEdge = -1;
	for (const int u : nodesWithExits.keys()) {
		QSet<int> edgesToRemove;

		int u1 = nodesWithExits[u];
		for (int edge : mFollowers2[u][u1]) {
			if (oneSavedEdge == -1) {
				oneSavedEdge = edge;
			}
			edgesToRemove.insert(edge);
		}


		if (u1 != commonExit) {
			for (int edge : mFollowers2[u1][commonExit]) {
				edgesToRemove.insert(edge);
			}
		}

		bool needToUpdateHead = u == v;
		QMap<QString, int> vertecesRoles;
		vertecesRoles["condition"] = u;
		vertecesRoles["then"] = nodesWithExits[u];
		vertecesRoles["exit"] = commonExit;
		reduceConditionAndAddBreak(edgesToRemove, vertecesRoles);
		if (needToUpdateHead) {
			v = mVertecesNumber - 1;
		}
	}

	if (commonExit != -1) {
		mFollowers2[v][commonExit].push_back(oneSavedEdge);
		mPredecessors2[commonExit][v].push_back(oneSavedEdge);
	}

	return true;
	*/

}

bool Structurizator::isHeadOfCycle(int v, QSet<int> &reachUnder)
{
	QQueue<int> queueForReachUnder;

	for (const int u : mPredecessors[v]) {
		if (mDominators[u].contains(v)) {
			queueForReachUnder.push_back(u);
		}
	}

	while (!queueForReachUnder.empty()) {
		int u = queueForReachUnder.front();
		queueForReachUnder.pop_front();
		reachUnder.insert(u);
		for (const int w : mPredecessors[u]) {
			if (mDominators[w].contains(v) && !reachUnder.contains(w)) {
				queueForReachUnder.push_back(w);
			}
		}
	}

	return !reachUnder.isEmpty();
}

bool Structurizator::findCommonExit(QSet<int> &reachUnder, QMap<int, int> &nodesWithExits, int &commonExit)
{
	commonExit = -1;
	QSet<int> exits;

	for (const int u : reachUnder) {
		for (const int w : mFollowers[u]) {
			if (!reachUnder.contains(w)) {
				if (exits.contains(w)) {
					if (commonExit != -1 && commonExit != w) {
						return false;
					}
					commonExit = w;
				}

				if (nodesWithExits.contains(u)) {
					return false;
				}

				exits.insert(w);
				nodesWithExits[u] = w;
			}
		}
	}

	if (commonExit != -1) {
		return true;
	}

	QList<int> regionToFindCommonChild;
	for (const int exitNumber : exits) {
		if (outgoingEdgesNumber(exitNumber) == 1) {
			regionToFindCommonChild.append(exitNumber);
		} else if (outgoingEdgesNumber(exitNumber) > 1) {
			if (commonExit == -1) {
				// we have found post-cycle execution point
				commonExit = exitNumber;
			} else if (commonExit != exitNumber) {
				// each cycle can have at most 1 point to transfer execution
				return false;
			}
		}
	}

	if (commonExit != -1) {
		return true;
	}

	// assume that one exit point is commonChild
	if (regionToFindCommonChild.size() == 1) {
		commonExit = regionToFindCommonChild.first();
		return true;
	}

	for (const int exitNumber : regionToFindCommonChild) {
		for (const int postExit : mFollowers[exitNumber]) {
			if (commonExit == -1) {
				commonExit = postExit;
			} else if (commonExit != postExit) {
				return false;
			}
		}
	}

	return true;
}

bool Structurizator::checkCommonExit(int commonExit, const QMap<int, int> &nodesWithExits)
{
	for (const int exit : nodesWithExits.values()) {
		if (commonExit == exit) {
			continue;
		}

		if (incomingEdgesNumber(exit) != 1 || outgoingEdgesNumber(exit) >= 2) {
			return false;
		}

		if (outgoingEdgesNumber(exit) == 1 && commonExit != mFollowers[exit].first()) {
			return false;
		}
	}

	return true;
}

void Structurizator::reduceBlock(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	BlockNode *block = new BlockNode(mTrees[vertecesRoles["block1"]], mTrees[vertecesRoles["block2"]], this);

	appendVertex(block, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceIfThenElse(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	IfNode *ifNode = new IfNode(mTrees[vertecesRoles["condition"]], mTrees[vertecesRoles["then"]], mTrees[vertecesRoles["else"]], this);

	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

void Structurizator::reduceIfThen(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	IfNode *ifNode = new IfNode(mTrees[vertecesRoles["condition"]], mTrees[vertecesRoles["then"]], nullptr, this);

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

	SwitchNode *switchNode = new SwitchNode(mTrees[v], branches);

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

void Structurizator::reduceConditionsWithBreaks(int v, QMap<int, int> &nodesWithExits, int commonExit)
{
	Q_UNUSED(v)

	for (const int u : nodesWithExits.keys()) {
		int exit = nodesWithExits[u];
		if (outgoingEdgesNumber(u) > 2) {
			// here we deal with switch
			addAdditionalConditionWithBreak(u, exit, commonExit);
		} else {
			// here we deal with if or switch with 2 outgoing branches
			reduceSimpleIfWithBreak(u, exit, commonExit);
		}
	}
}

void Structurizator::reduceSimpleIfWithBreak(int conditionVertex, int thenVertex, int exitVertex)
{

	IfWithBreakNode *ifWithBreakNode = new IfWithBreakNode(mTrees[conditionVertex]
																, thenVertex == exitVertex ? nullptr : mTrees[thenVertex]
																, this);

	QSet<int> edgesToRemove = {mMapEdgeNumberToVerteces[QPair<int, int>(conditionVertex, thenVertex)]};
	QSet<int> verteces = {conditionVertex};

	if (thenVertex != exitVertex) {
		verteces.insert(thenVertex);
		if (mFollowers[thenVertex].contains(exitVertex)) {
			edgesToRemove.insert(mMapEdgeNumberToVerteces[QPair<int, int>(thenVertex, exitVertex)]);
		}
	}

	appendVertex(ifWithBreakNode, edgesToRemove, verteces);
}

void Structurizator::addAdditionalConditionWithBreak(int conditionVertex, int thenVertex, int exitVertex)
{
	Q_UNUSED(conditionVertex)
	Q_UNUSED(thenVertex)
	Q_UNUSED(exitVertex)
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

			int newV = verteces.contains(v) ? newNodeNumber : v;
			int newU = verteces.contains(u) ? newNodeNumber : u;

			if (newU == newNodeNumber || newV == newNodeNumber) {
				// removing old information
				mFollowers[v].removeAll(u);
				mPredecessors[u].removeAll(v);

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

	mDominators[mStartVertex] = { mStartVertex };

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
			return;
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
		mTrees[v] = new SimpleNode(mMapIdToInt.key(v), this);
	}
}

void Structurizator::dfs(int v, int &currentTime, QMap<int, bool> &used)
{
	used[v] = true;
	for (const int u : mFollowers[v]) {
		if (!used[u]) {
			dfs(u, currentTime, used);
		}
	}

	mPostOrder[v] = currentTime;
	currentTime++;
}

void Structurizator::appendVertex(Node *node, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	QSet<int> verteces = vertecesRoles.values().toSet();
	appendVertex(node, edgesToRemove, verteces);
}

void Structurizator::appendVertex(Node *node, QSet<int> &edgesToRemove, QSet<int> &verteces)
{
	mTrees[mVertecesNumber] = node;
	mVerteces.insert(mVertecesNumber);

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
