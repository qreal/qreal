#include "constraintsBlockDiagramMetamodelPlugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

Q_EXPORT_PLUGIN2(constraintsBlockDiagramMetamodelPlugin, constraints::ConstraintsPlugin)

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
	if (((mConstraintsBlockDiagram_1.languageName() == languageName) || (mConstraintsBlockDiagram_1.languageName() == "AllLanguages")) && (mConstraintsBlockDiagram_1.elementsNames().contains("AllEdges") || mConstraintsBlockDiagram_1.elementsNames().contains("AllNodes") || mConstraintsBlockDiagram_1.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsBlockDiagram_1.check(element, logicalApi, editorManager));
	}
	if (((mConstraintsBlockDiagram_2.languageName() == languageName) || (mConstraintsBlockDiagram_2.languageName() == "AllLanguages")) && (mConstraintsBlockDiagram_2.elementsNames().contains("AllEdges") || mConstraintsBlockDiagram_2.elementsNames().contains("AllNodes") || mConstraintsBlockDiagram_2.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsBlockDiagram_2.check(element, logicalApi, editorManager));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "BlockDiagramMetamodel";
}

QString ConstraintsPlugin::id() const
{
	return "BlockDiagramMetamodelConstraints_2";
}
