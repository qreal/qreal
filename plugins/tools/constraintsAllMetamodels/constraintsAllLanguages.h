#pragma once
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>
#include <qrgui/plugins/constraintsPluginInterface/constraintsPluginInterface.h>

namespace constraints {

class ConstraintsAllLanguages
{
public:
	ConstraintsAllLanguages();
	virtual ~ConstraintsAllLanguages();

	qReal::CheckStatus checkAllEdges_one(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkAllEdges(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;

private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

