#include "constraintsBlockDiagram_2.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsBlockDiagram_2::ConstraintsBlockDiagram_2()
{
}

ConstraintsBlockDiagram_2::~ConstraintsBlockDiagram_2()
{
}

qReal::CheckStatus ConstraintsBlockDiagram_2::checkConditionNode_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
		bool propertyNodeRes_4 = true;
	if (logicalApi.hasProperty(element, "condition")) {
		propertyNodeRes_4 = (logicalApi.property(element, "condition").toString() == "qwerty");
	}
	bool propertyNodeRes_5 = true;
	if (logicalApi.hasProperty(element, "flag")) {
		propertyNodeRes_5 = (logicalApi.property(element, "flag").toString() == "true");
	}
	bool propertyNodeRes_6 = true;
	if (logicalApi.hasProperty(element, "condition")) {
		propertyNodeRes_6 = (logicalApi.property(element, "condition").toString() == "asd");
	}
		bool nodeRes_1 = (true && true && ((propertyNodeRes_4 || propertyNodeRes_5 || propertyNodeRes_6)));
	res = nodeRes_1;

	return qReal::CheckStatus(res, "conditionNodeError", qReal::CheckStatus::critical);
}


QList<qReal::CheckStatus> ConstraintsBlockDiagram_2::checkConditionNode(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkConditionNode_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsBlockDiagram_2::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;

	if (element.element() == "ConditionNode") {
		checkings.append(checkConditionNode(element, logicalApi));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsBlockDiagram_2::languageName() const
{
	return "BlockDiagram";
}

QList<QString> ConstraintsBlockDiagram_2::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("ConditionNode");

	return elementsList;
}
