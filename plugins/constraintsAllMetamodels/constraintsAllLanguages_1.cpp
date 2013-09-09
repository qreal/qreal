#include "constraintsAllLanguages_1.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsAllLanguages_1::ConstraintsAllLanguages_1()
{
}

ConstraintsAllLanguages_1::~ConstraintsAllLanguages_1()
{
}

qReal::CheckStatus ConstraintsAllLanguages_1::checkAllEdges_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
	qReal::Id newEndNodeName_1 = logicalApi.to(element);
	bool endNodeRes_2 = true;
	bool endNodeRes_1 = (newEndNodeName_1 != qReal::Id::rootId());
	qReal::Id newBeginNodeName_1 = logicalApi.from(element);
	bool beginNodeRes_2 = true;
	bool beginNodeRes_1 = (newBeginNodeName_1 != qReal::Id::rootId());
	bool allEdgesRes_1 = endNodeRes_2 && endNodeRes_1 && beginNodeRes_2 && beginNodeRes_1;
	res = allEdgesRes_1;

	return qReal::CheckStatus(res, "fail_text", qReal::CheckStatus::warning);
}


QList<qReal::CheckStatus> ConstraintsAllLanguages_1::checkAllEdges(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkAllEdges_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsAllLanguages_1::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;
	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(element);

	if(isNodeOrEdge == qReal::EditorManagerInterface::edge) {
		checkings.append(checkAllEdges(element, logicalApi));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsAllLanguages_1::languageName() const
{
	return "AllLanguages";
}

QList<QString> ConstraintsAllLanguages_1::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("AllEdges");

	return elementsList;
}
