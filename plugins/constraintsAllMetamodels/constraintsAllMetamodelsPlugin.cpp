#include "constraintsAllMetamodelsPlugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

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
	if (((mConstraintsAllLanguages.languageName() == languageName)
			|| (mConstraintsAllLanguages.languageName() == "AllLanguages"))
		&& (mConstraintsAllLanguages.elementsNames().contains("AllEdges")
			|| mConstraintsAllLanguages.elementsNames().contains("AllNodes")
			|| mConstraintsAllLanguages.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsAllLanguages.check(element, logicalApi, editorManager));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "AllMetamodels";
}

QString ConstraintsPlugin::id() const
{
	return "AllMetamodelsConstraints";
}
