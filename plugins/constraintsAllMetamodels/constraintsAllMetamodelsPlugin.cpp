#include "constraintsAllMetamodelsPlugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

Q_EXPORT_PLUGIN2(constraintsAllMetamodelsPlugin, constraints::ConstraintsPlugin)

using namespace constraints;

ConstraintsPlugin::ConstraintsPlugin()
{
}

ConstraintsPlugin::~ConstraintsPlugin()
{
}

QList<qReal::CheckStatus> ConstraintsPlugin::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	QString const elementName = element.element();
	QString const languageName = element.diagram();
	if (((mConstraintsAllLanguages_1.languageName() == languageName) || (mConstraintsAllLanguages_1.languageName() == "AllLanguages")) && (mConstraintsAllLanguages_1.elementsNames().contains("AllEdges") || mConstraintsAllLanguages_1.elementsNames().contains("AllNodes") || mConstraintsAllLanguages_1.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsAllLanguages_1.check(element, logicalApi, editorManager));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "AllMetamodels";
}

QString ConstraintsPlugin::id() const
{
	return "ConstraintsForAllLanguages";
}
