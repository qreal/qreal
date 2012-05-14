#pragma once

#include "../../../qrgui/pluginManager/editorManagerInterface.h"
#include "constraintsRobotsDiagram_1.h"
#include "constraintsRobotsDiagram_2.h"

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
	ConstraintsRobotsDiagram_1 mConstraintsRobotsDiagram_1;
	ConstraintsRobotsDiagram_2 mConstraintsRobotsDiagram_2;

};

}
