#include <QtGui/QMessageBox>
#include "editor_handmade.h"

#include "../../qrgui/umllib/uml_nodeelement.h"
#include "kroki.h"

// i.e. qrm:///TestEditor/UseCase/Actor

Q_EXPORT_PLUGIN2(qreal_editors, HandmadePlugin)

HandmadePlugin::HandmadePlugin()
{
	initPlugin();
}

void HandmadePlugin::initPlugin()
{
	diagramNameMap["UseCase"] = "Use case diagram";
	diagramNameMap["Orthodox"] = "Orthodox diagram";

	elementsNameMap["UseCase"]["UseCase"] = "Use case";
	elementsNameMap["UseCase"]["Actor"] = "Actor";
	elementsNameMap["Orthodox"]["Kroki"] = "Kroki";
	elementsNameMap["Orthodox"]["Swastika"] = "Sign of Life";

//	QMessageBox::information(0, "I'm a QReal Plugin", "Handmade Plugin loaded");
}

QStringList HandmadePlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList HandmadePlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QIcon HandmadePlugin::getIcon(QString const &diagram, QString const &element) const
{
	return QIcon(new SdfIconEngineV2(":/TestEditor/" + diagram + "/" + element + ".sdf"));
}

QString HandmadePlugin::editorName() const
{
	return "Test handmade editor";
}

QString HandmadePlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString HandmadePlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

UML::Element* HandmadePlugin::getGraphicalObject(QString const &diagram, QString const &element) const
{
	if (element == "Kroki")
		return new UML::Kroki();
	return new UML::Kroki();
}
