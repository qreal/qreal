#include "constraintsRobotsMetamodelPlugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

Q_EXPORT_PLUGIN2(constraintsRobotsMetamodelPlugin, constraints::ConstraintsPlugin)

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
	if (((mConstraintsRobotsDiagram_1.languageName() == languageName) || (mConstraintsRobotsDiagram_1.languageName() == "AllLanguages")) && (mConstraintsRobotsDiagram_1.elementsNames().contains("AllEdges") || mConstraintsRobotsDiagram_1.elementsNames().contains("AllNodes") || mConstraintsRobotsDiagram_1.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsRobotsDiagram_1.check(element, logicalApi, editorManager));
	}
	if (((mConstraintsRobotsDiagram_2.languageName() == languageName) || (mConstraintsRobotsDiagram_2.languageName() == "AllLanguages")) && (mConstraintsRobotsDiagram_2.elementsNames().contains("AllEdges") || mConstraintsRobotsDiagram_2.elementsNames().contains("AllNodes") || mConstraintsRobotsDiagram_2.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsRobotsDiagram_2.check(element, logicalApi, editorManager));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "RobotsMetamodel";
}

QString ConstraintsPlugin::id() const
{
	return "constraintsMetamodelForRobots";
}
