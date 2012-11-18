#include "constraintsBlockDiagram_1.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsBlockDiagram_1::ConstraintsBlockDiagram_1()
{
}

ConstraintsBlockDiagram_1::~ConstraintsBlockDiagram_1()
{
}

qReal::CheckStatus ConstraintsBlockDiagram_1::checkControlFlow_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
			qReal::Id newBeginNodeName_1 = logicalApi.from(element);
		bool propertyNodeRes_2 = true;
		if (logicalApi.hasProperty(newBeginNodeName_1, "finished")) {
			propertyNodeRes_2 = (logicalApi.property(newBeginNodeName_1, "finished").toString() == "true");
		}
			bool beginNodeRes_2 = propertyNodeRes_2;
		bool beginNodeRes_1 = (newBeginNodeName_1 != qReal::Id::rootId());
			bool allEdgesRes_1 = (beginNodeRes_2 && beginNodeRes_1);
	res = allEdgesRes_1;

	return qReal::CheckStatus(res, "controlFlowError", qReal::CheckStatus::critical);
}


QList<qReal::CheckStatus> ConstraintsBlockDiagram_1::checkControlFlow(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkControlFlow_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsBlockDiagram_1::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;

	if (element.element() == "ControlFlow") {
		checkings.append(checkControlFlow(element, logicalApi));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsBlockDiagram_1::languageName() const
{
	return "BlockDiagram";
}

QList<QString> ConstraintsBlockDiagram_1::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("ControlFlow");

	return elementsList;
}
