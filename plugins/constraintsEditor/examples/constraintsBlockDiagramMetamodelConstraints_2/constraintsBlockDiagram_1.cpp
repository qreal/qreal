#include "constraintsBlockDiagram_1.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsBlockDiagram_1::ConstraintsBlockDiagram_1()
{
}

ConstraintsBlockDiagram_1::~ConstraintsBlockDiagram_1()
{
}

qReal::CheckStatus ConstraintsBlockDiagram_1::checkAction_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
		bool elementsSelectionByPropertyRes_1 = false;
	if (logicalApi.hasProperty(element, "finished")) {
		elementsSelectionByPropertyRes_1 = (logicalApi.property(element, "finished").toString() == "true");
	}
	if (elementsSelectionByPropertyRes_1) {
		qReal::IdList newIncomingLinksNamesList_1 = logicalApi.incomingLinks(element);
		bool mainIncomingLinksRes_2 = true;
		foreach (qReal::Id const &edgeIncomingLinks_1, newIncomingLinksNamesList_1) {
			bool propertyNodeRes_2 = true;
			if (logicalApi.hasProperty(edgeIncomingLinks_1, "type")) {
				propertyNodeRes_2 = (logicalApi.property(edgeIncomingLinks_1, "type").toString() == "true");
			}
				bool incomingLinksRes_2 = propertyNodeRes_2;
			mainIncomingLinksRes_2 = mainIncomingLinksRes_2 && incomingLinksRes_2;
		}
			bool allNodesRes_1 = mainIncomingLinksRes_2;
	res = allNodesRes_1;
	}

	return qReal::CheckStatus(res, "actionError", qReal::CheckStatus::critical);
}


QList<qReal::CheckStatus> ConstraintsBlockDiagram_1::checkAction(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkAction_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsBlockDiagram_1::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;

	if (element.element() == "Action") {
		checkings.append(checkAction(element, logicalApi));
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
	elementsList.append("Action");

	return elementsList;
}
