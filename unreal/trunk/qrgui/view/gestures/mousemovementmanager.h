#pragma once
#include "IKeyManager.h"
#include "keymanager.h"
#include "../../kernel/ids.h"
#include "../../editorManager/editorManager.h"
#include <QPoint>
#include <QList>
#include <QString>

class MouseMovementManager
{
public:
    MouseMovementManager(qReal::Id diagram, qReal::EditorManager editorManager);
    void setDiagram(qReal::Id * diagram);
    void addPoint(QPoint const & point);
    qReal::Id getObject();
    static QList<QPoint> stringToPath(QString const &str);

private:
    static QPoint parsePoint(QString const &str);
    QList<QPoint> mPath;
    IKeyManager * mKeyManager;
    KeyManager mKeyStringManager;
    qReal::EditorManager * mEditorManager;
    qReal::Id * mDiagram;
};
