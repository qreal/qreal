#pragma once

#include "@@pathToQReal@@/qrgui/pluginManager/editorManagerInterface.h"
@@includeConstraintsDiagramFiles@@
namespace constraints {

class ConstraintsPlugin : public QObject, public qReal::ConstraintsPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ConstraintsPluginInterface)

public:
	ConstraintsPlugin();
	virtual ~ConstraintsPlugin();

	virtual qReal::CheckStatus check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager);
	virtual QString metamodelName() const;
	virtual QString id() const;

private:
@@privateFieldsOfConstraintsDigrams@@
};

}
