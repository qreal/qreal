#ifndef HANDMADE_H
#define HANDMADE_H

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>

#include "editorinterface.h"

class HandmadePlugin : public QObject, public EditorInterface
{
    Q_OBJECT
    Q_INTERFACES(EditorInterface)
    
public:

    HandmadePlugin();

    void initPlugin();
    
    QString name() const { return "usecase"; };

    QStringList diagrams() const;
    QStringList elements() const;

    QIcon getIcon(QString element) const;
    QString getName(QString element) const;

private:
    QMap<QString, QIcon> iconMap;
    QMap<QString, QString> friendlyNameMap;
};

#endif
