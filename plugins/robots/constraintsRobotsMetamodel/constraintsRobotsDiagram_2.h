#pragma once
#include "../../../qrgui/pluginManager/editorManagerInterface.h"
#include "../../../qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class ConstraintsRobotsDiagram_2
{
public:
	ConstraintsRobotsDiagram_2();
	virtual ~ConstraintsRobotsDiagram_2();

	qReal::CheckStatus checkEnginesForward_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);
	qReal::CheckStatus checkEnginesBackward_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkEnginesForward(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);
	QList<qReal::CheckStatus> checkEnginesBackward(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

