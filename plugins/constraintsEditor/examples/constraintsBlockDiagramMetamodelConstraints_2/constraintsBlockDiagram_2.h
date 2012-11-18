#pragma once
#include "../../../../qrgui/pluginManager/editorManagerInterface.h"
#include "../../../../qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class ConstraintsBlockDiagram_2
{
public:
	ConstraintsBlockDiagram_2();
	virtual ~ConstraintsBlockDiagram_2();

	qReal::CheckStatus checkConditionNode_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkConditionNode(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

