#pragma once

#include "../../../../qrgui/pluginManager/editorManagerInterface.h"
#include "constraintsBlockDiagram_1.h"

namespace constraints {

class ConstraintsPlugin : public QObject, public qReal::ConstraintsPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ConstraintsPluginInterface)

public:
	ConstraintsPlugin();
	virtual ~ConstraintsPlugin();

	virtual QList<qReal::CheckStatus> check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);
	virtual QString metamodelName() const;
	virtual QString id() const;

private:
	ConstraintsBlockDiagram_1 mConstraintsBlockDiagram_1;

};

}
