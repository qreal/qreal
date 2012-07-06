#include "constraintsRobotsDiagram_1.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsRobotsDiagram_1::ConstraintsRobotsDiagram_1()
{
}

ConstraintsRobotsDiagram_1::~ConstraintsRobotsDiagram_1()
{
}

qReal::CheckStatus ConstraintsRobotsDiagram_1::checkRobotsDiagramNode_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
		int tempCountChildren_1 = 0;
	qReal::IdList newChildrenNamesList_1 = logicalApi.children(element);
	bool mainChildrenRes_2 = true;
	foreach (qReal::Id const &nodeChildren_1, newChildrenNamesList_1) {
		bool childrenSelectionRes_1 = false;
		if (nodeChildren_1 != qReal::Id::rootId()) {
			childrenSelectionRes_1 = (nodeChildren_1.element() == "InitialBlock");
		}
		if (childrenSelectionRes_1) {
			bool childrenRes_2 = true;
		mainChildrenRes_2 = mainChildrenRes_2 && childrenRes_2;
		tempCountChildren_1++;
		}
	}
	bool countChildrenRes_1 = (tempCountChildren_1 <= 1);
		bool nodeRes_1 = mainChildrenRes_2 && countChildrenRes_1;
	res = nodeRes_1;

	return qReal::CheckStatus(res, "Expected no more than one InitialBlock on the RobotsDiagram", qReal::CheckStatus::critical);
}
qReal::CheckStatus ConstraintsRobotsDiagram_1::checkEnginesStop_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
		qReal::IdList newIncomingNodesNamesList_1 = logicalApi.incomingNodes(element);
	bool mainIncomingNodesRes_2 = true;
	foreach (qReal::Id const &nodeIncomingNodes_1, newIncomingNodesNamesList_1) {
		bool propertyNodeRes_2 = true;
		if (nodeIncomingNodes_1 != qReal::Id::rootId()) {
			propertyNodeRes_2 = (nodeIncomingNodes_1.element() != "EnginesForward");
		}
		bool propertyNodeRes_3 = true;
		if (nodeIncomingNodes_1 != qReal::Id::rootId()) {
			propertyNodeRes_3 = (nodeIncomingNodes_1.element() != "EnginesBackward");
		}
			bool incomingNodesRes_2 = propertyNodeRes_2 && propertyNodeRes_3;
		mainIncomingNodesRes_2 = mainIncomingNodesRes_2 && incomingNodesRes_2;
	}
		bool nodeRes_1 = mainIncomingNodesRes_2;
	res = nodeRes_1;

	return qReal::CheckStatus(res, "Before EnginesStop you can't have EnginesBackward or EnginesForward", qReal::CheckStatus::critical);
}


QList<qReal::CheckStatus> ConstraintsRobotsDiagram_1::checkEnginesStop(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkEnginesStop_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}
QList<qReal::CheckStatus> ConstraintsRobotsDiagram_1::checkRobotsDiagramNode(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkRobotsDiagramNode_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsRobotsDiagram_1::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	mEditorManager = &editorManager;

	if (element.element() == "RobotsDiagramNode") {
		return checkRobotsDiagramNode(element, logicalApi);
	}
	if (element.element() == "EnginesStop") {
		return checkEnginesStop(element, logicalApi);
	}

	return qReal::CheckStatus::defaultCheckStatusAsList();
}

QString ConstraintsRobotsDiagram_1::languageName() const
{
	return "RobotsDiagram";
}

QList<QString> ConstraintsRobotsDiagram_1::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("RobotsDiagramNode");
	elementsList.append("EnginesStop");

	return elementsList;
}
