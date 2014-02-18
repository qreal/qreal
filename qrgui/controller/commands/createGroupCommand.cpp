#include "createGroupCommand.h"

#include "controller/commands/insertIntoEdgeCommand.h"

using namespace qReal::commands;

CreateGroupCommand::CreateGroupCommand(EditorViewScene * const scene
		, models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, Id const &logicalParent
		, Id const &graphicalParent
		, Id const &id
		, bool isFromLogicalModel
		, QPointF const &position)
	: mScene(scene)
	, mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mLogicalParent(logicalParent)
	, mGraphicalParent(graphicalParent)
	, mId(id)
	, mIsFromLogicalModel(isFromLogicalModel)
	, mPosition(position)
	, mPattern(graphicalApi.editorManagerInterface().getPatternByName(id.element()))
{
	QPointF const size = mPattern.size();

	// Pattern nodes create may have hierarchic structure. So we must create them in correct order
	// (parent first, child after). Cycles in hierarchy and nodes with incorrect parent id are fully ignored
	QList<GroupNode> toCreate = mPattern.nodes();
	QSet<QString> consideredNodes;
	QMap<QString, Id> createdNodesIds;
	// If group node has no parent then it has 'global' one
	createdNodesIds[QString()] = graphicalParent;
	bool somethingChangedThisIteration = true;
	while (!toCreate.isEmpty() && somethingChangedThisIteration) {
		somethingChangedThisIteration = false;
		for (GroupNode const &node : toCreate) {
			if (!node.parent.isEmpty() && !consideredNodes.contains(node.parent)) {
				continue;
			}

			Id const element(id.editor(), id.diagram(), node.type, QUuid::createUuid().toString());
			createdNodesIds[node.id] = element;
			if (node.id == mPattern.rootNode()) {
				mRootId = element;
			}

			QPointF const nodePos(position.x() - size.x() / 2 + node.position.x()
					, position.y() + node.position.y());
			CreateElementCommand *createNodeCommand = new CreateElementCommand(
					logicalApi, graphicalApi, logicalParent
					, createdNodesIds[node.parent], element, isFromLogicalModel
					, mLogicalApi.editorManagerInterface().friendlyName(element.type()), nodePos);
			mNodeCommands[node.id] = createNodeCommand;
			addPreAction(createNodeCommand);
			consideredNodes << node.id;
			toCreate.removeAll(node);
			somethingChangedThisIteration = true;
		}
	}
	// TODO: display here error if toCreate still non-empty

	for (GroupEdge const &edge : mPattern.edges()) {
		Id const element(id.editor(), id.diagram(), edge.type, QUuid::createUuid().toString());
		CreateElementCommand *createEdgeCommand = new CreateElementCommand(
					logicalApi, graphicalApi, logicalParent, graphicalParent, element, isFromLogicalModel
					, mLogicalApi.editorManagerInterface().friendlyName(element.type()), QPointF());
		mEdgeCommands.append(createEdgeCommand);
		addPreAction(createEdgeCommand);
	}
}

bool CreateGroupCommand::execute()
{
	// Elements themselves were already created in pre-actions
	QMap<QString, Id> nodes;
	for (QString const &idInGroup : mNodeCommands.keys()) {
		CreateElementCommand const *createNodeCommand = mNodeCommands[idInGroup];
		Id const newElemId = createNodeCommand->result();
		nodes.insert(idInGroup, newElemId);
	}

	for (int i = 0; i < mEdgeCommands.count(); ++i) {
		CreateElementCommand const *createEdgeCommand = mEdgeCommands[i];
		GroupEdge const groupEdge = mPattern.edges()[i];
		Id const newEdgeId = createEdgeCommand->result();
		mGraphicalApi.setFrom(newEdgeId, nodes.value(groupEdge.from));
		mGraphicalApi.setTo(newEdgeId, nodes.value(groupEdge.to));
		if (mScene) {
			mScene->reConnectLink(mScene->getEdgeById(newEdgeId));
		}
	}

	if (mScene) {
		InsertIntoEdgeCommand *insertCommand = new InsertIntoEdgeCommand(*mScene, mLogicalApi, mGraphicalApi
				, nodes.value(mPattern.inNode()), nodes.value(mPattern.outNode()), mGraphicalParent, mPosition
				, mPattern.size(), mIsFromLogicalModel);
		insertCommand->redo();
	}

	return true;
}

bool CreateGroupCommand::restoreState()
{
	return true;
}

Id CreateGroupCommand::rootId() const
{
	return mRootId;
}
