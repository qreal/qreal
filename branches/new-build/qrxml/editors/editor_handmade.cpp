#include "editor_handmade.h"

void HandmadePlugin::initPlugin()
{
    QMessageBox::information(0, "QReal Plugin", "Handmade Plugin loaded");
}

QStringList HandmadePlugin::diagrams() const
{
    QStringList myElements;
    myElements << "Use Case";
    return myElements;
}

QStringList HandmadePlugin::elements(int idx) const
{
    return QStringList();
}

Q_EXPORT_PLUGIN2(qreal_editors, HandmadePlugin)
