#ifndef HANDMADE_H
#define HANDMADE_H

#include <QObject>
#include <QStringList>
#include <QMessageBox>

#include "editorinterface.h"

class HandmadePlugin : public QObject, public EditorInterface
{
    Q_OBJECT
    Q_INTERFACES(EditorInterface)

public:
    void initPlugin();
    QStringList diagrams() const;
    QStringList elements(int idx) const;
};

#endif
