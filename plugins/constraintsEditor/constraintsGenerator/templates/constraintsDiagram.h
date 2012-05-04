#pragma once
#include "@@pathToQReal@@/qrgui/pluginManager/editorManagerInterface.h"
#include "@@pathToQReal@@/qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class Constraints@@diagramName@@
{
public:
	Constraints@@diagramName@@();
	virtual ~Constraints@@diagramName@@();

@@optionalChecksForElemets@@
@@mainChecksForElemets@@
	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

