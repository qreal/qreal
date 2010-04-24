#ifndef @@NAME@@_H
#define @@NAME@@_H

#include <QObject>
#include <QStringList>
#include <QMessageBox>

#include "editorInterface.h"

class EditorTest_@@NAME@@_Plugin : public QObject, public EditorInterface
{
    Q_OBJECT
    Q_INTERFACES(EditorInterface)

public:
    QStringList elements() const;
    void testLoad(QWidget *parent);
};

#endif
