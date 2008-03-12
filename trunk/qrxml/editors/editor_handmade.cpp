#include <QtGui/QMessageBox>
#include "editor_handmade.h"

Q_EXPORT_PLUGIN2(qreal_editors, HandmadePlugin)

HandmadePlugin::HandmadePlugin()
{
    initPlugin();
}

void HandmadePlugin::initPlugin()
{
    iconMap["/usecase/ucdiagram/actor"] = QIcon(":/plugins/usecase/ucdiagram/actor.svg");
    iconMap["/usecase/ucdiagram/usecase"] = QIcon(":/plugins/usecase/ucdiagram/usecase.svg");

    friendlyNameMap["/usecase/ucdiagram"] = "Use case";
    friendlyNameMap["/usecase/ucdiagram/actor"] = "Actor";
    friendlyNameMap["/usecase/ucdiagram/usecase"] = "Use case";

    QMessageBox::information(0, "QReal Plugin", "Handmade Plugin loaded");
}

QStringList HandmadePlugin::diagrams() const
{
    QStringList myElements;
    myElements << "/usecase/ucdiagram";
    return myElements;
}

QStringList HandmadePlugin::elements() const
{
    QStringList myElements;
    myElements << "/usecase/ucdiagram/actor"
		<< "/usecase/ucdiagram/usecase";
    return myElements;
}

QIcon HandmadePlugin::getIcon(QString element) const
{
    return iconMap[element];
}

QString HandmadePlugin::getName(QString element) const
{
    return friendlyNameMap[element];
}
