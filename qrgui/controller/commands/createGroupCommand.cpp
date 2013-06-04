#include "createGroupCommand.h"

using namespace qReal::commands;

CreateGroupCommand::CreateGroupCommand(EditorViewScene * const scene
		, models::LogicalModelAssistApi * const logicalApi
		, models::GraphicalModelAssistApi * const graphicalApi
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
	, mPattern(graphicalApi->editorManagerInterface()->getPatternByName(id.element()))
{
	QPointF size = mPattern.size();
	foreach (GroupNode const &node, mPattern.nodes()) {
		Id const element(id.editor(), id.diagram(), node.type, QUuid::createUuid().toString());
		QPointF const nodePos(position.x()- size.x()/2 + node.position.x()
						, position.y() + node.position.y());
		CreateElementCommand *createNodeCommand = new CreateElementCommand(
					logicalApi, graphicalApi, logicalParent, graphicalParent, element
					, isFromLogicalModel, "(" + node.type + ")", nodePos);
		mNodeCommands.append(createNodeCommand);
		addPreAction(createNodeCommand);
	}

	foreach (GroupEdge const &edge, mPattern.edges()) {
		Id const element(id.editor(), id.diagram(), edge.type, QUuid::createUuid().toString());
		CreateElementCommand *createEdgeCommand = new CreateElementCommand(
					logicalApi, graphicalApi, logicalParent, graphicalParent, element
					, isFromLogicalModel, "(" + edge.type + ")", QPointF());
		mEdgeCommands.append(createEdgeCommand);
		addPreAction(createEdgeCommand);
	}
}

bool CreateGroupCommand::execute()
{
	QList<NodeElement *> elements;
	QMap<QString, Id> nodes;
	for (int i = 0; i < mNodeCommands.count(); ++i) {
		CreateElementCommand const *createNodeCommand = mNodeCommands[i];
		Id const newElemId = createNodeCommand->result();
		nodes.insert(mPattern.nodes()[i].id, newElemId);
		elements.append(mScene->getNodeById(newElemId));
	}
	for (int i = 0; i < mEdgeCommands.count(); ++i) {
		CreateElementCommand const *createEdgeCommand = mEdgeCommands[i];
		GroupEdge const groupEdge = mPattern.edges()[i];
		Id const newEdgeId = createEdgeCommand->result();
		mGraphicalApi->setFrom(newEdgeId, nodes.value(groupEdge.from));
		mGraphicalApi->setTo(newEdgeId, nodes.value(groupEdge.to));
		mScene->getNodeById(nodes.value(groupEdge.to))->connectLinksToPorts();
		mScene->reConnectLink(mScene->getEdgeById(newEdgeId));
	}
	mScene->insertElementIntoEdge(nodes.value(mPattern.inNode()), nodes.value(mPattern.outNode())
			, mGraphicalParent, mIsFromLogicalModel, mPosition, mPattern.size(), elements);
	return true;
}

bool CreateGroupCommand::restoreState()
{
	return true;
}
