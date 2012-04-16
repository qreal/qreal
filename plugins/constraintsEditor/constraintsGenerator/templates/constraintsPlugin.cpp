#include "constraintsPlugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

Q_EXPORT_PLUGIN2(constraintsPlugin, constraints::ConstraintsPlugin)

using namespace constraints;

ConstraintsPlugin::ConstraintsPlugin()
{
}

ConstraintsPlugin::~ConstraintsPlugin()
{
}

qReal::CheckStatus ConstraintsPlugin::check(qReal::IdList const &elements, qrRepo::LogicalRepoApi const &logicalApi) //asd //??
{
	QList<qReal::CheckStatus> checkings;
	foreach (qReal::Id element, elements) {
		QString const elementName = element.element();
		QString const languageName = element.diagram();
@@ifForMainCheckOfConstraintsDiagrams@@
	}
	return qReal::CheckStatus::resultCheckStatus(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "@@metaModelName@@";
}
