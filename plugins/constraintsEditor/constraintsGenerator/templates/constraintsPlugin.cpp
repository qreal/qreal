#include "constraints@@metamodelName@@Plugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

Q_EXPORT_PLUGIN2(constraints@@metamodelName@@Plugin, constraints::ConstraintsPlugin)

using namespace constraints;

ConstraintsPlugin::ConstraintsPlugin()
{
}

ConstraintsPlugin::~ConstraintsPlugin()
{
}

qReal::CheckStatus ConstraintsPlugin::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	QString const elementName = element.element();
	QString const languageName = element.diagram();
@@ifForMainCheckOfConstraintsDiagrams@@
	return qReal::CheckStatus::resultCheckStatus(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "@@metamodelName@@";
}

QString ConstraintsPlugin::id() const
{
	return "@@constraintsPluginId@@";
}
