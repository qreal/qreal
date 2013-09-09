#pragma once
#include "../../../../qrgui/pluginManager/editorManagerInterface.h"
#include "../../../../qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class ConstraintsBlockDiagram_1
{
public:
	ConstraintsBlockDiagram_1();
	virtual ~ConstraintsBlockDiagram_1();

	qReal::CheckStatus checkControlFlow_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkControlFlow(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

