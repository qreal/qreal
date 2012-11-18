#pragma once
#include "../../qreal/qrgui/pluginManager/editorManagerInterface.h"
#include "../../qreal/qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class ConstraintsAllLanguages_1
{
public:
	ConstraintsAllLanguages_1();
	virtual ~ConstraintsAllLanguages_1();

	qReal::CheckStatus checkAllNodes_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkAllNodes(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;
	
private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

