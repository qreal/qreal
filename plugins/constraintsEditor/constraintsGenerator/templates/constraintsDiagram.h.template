#pragma once
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>
#include <qrgui/plugins/constraintsPluginInterface/constraintsPluginInterface.h>

namespace constraints {

class Constraints@@diagramName@@
{
public:
	Constraints@@diagramName@@();
	virtual ~Constraints@@diagramName@@();

@@optionalChecksForElements@@
@@mainChecksForElements@@
	QList<qReal::CheckStatus> check(qReal::Id const &element
		, qrRepo::LogicalRepoApi const &logicalApi
		, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

