#include <QtGui/QMessageBox>
#include "editor_handmade.h"

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
	elementsNameMap["Orthodox"]["Reichsadler"] = "Aryan Eagle";
	elementsNameMap["Orthodox"]["Swastika"] = "Sign of Life";


//	QMessageBox::information(0, "I'm a QReal Plugin", "Handmade Plugin loaded");
}

QStringList HandmadePlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList HandmadePlugin::elements(QString diagram) const
{
	return elementsNameMap[diagram].keys();
}

QIcon HandmadePlugin::getIcon(QString element) const
{
    return QIcon();
}

QString HandmadePlugin::editorName() const
{
    return "Test handmade editor";
}

QString HandmadePlugin::diagramName(QString diagram) const
{
    return diagramNameMap[diagram];
}

QString HandmadePlugin::elementName(QString diagram, QString element) const
{
    return elementsNameMap[diagram][element];
}
