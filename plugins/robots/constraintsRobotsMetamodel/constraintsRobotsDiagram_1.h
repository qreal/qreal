#pragma once
#include "../../../qrgui/pluginManager/editorManagerInterface.h"
#include "../../../qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class ConstraintsRobotsDiagram_1
{
public:
	ConstraintsRobotsDiagram_1();
	virtual ~ConstraintsRobotsDiagram_1();

	qReal::CheckStatus checkRobotsDiagramNode_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);
	qReal::CheckStatus checkEnginesStop_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkRobotsDiagramNode(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);
	QList<qReal::CheckStatus> checkEnginesStop(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

