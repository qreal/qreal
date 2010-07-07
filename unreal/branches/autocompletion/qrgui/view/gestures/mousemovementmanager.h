#pragma once
#include "IKeyManager.h"
#include "keymanager.h"
#include "../../kernel/ids.h"
#include "../../editorManager/editorManager.h"
#include <QPointF>
#include <QList>
#include <QString>
#include <QMap>

class MouseMovementManager
{
public:
    MouseMovementManager(QList<qReal::Id> elements, qReal::EditorManager * editorManager);
    void setElements(QList<qReal::Id> const & elements);
    void addPoint(QPointF const & point);
    qReal::Id getObject();
    static QList<QPointF> stringToPath(QString const &str);

private:
    static QPointF parsePoint(QString const &str);
    void createMap();
    QList<QPointF> mPath;
    IKeyManager * mKeyManager;
    KeyManager mKeyStringManager;
    qReal::EditorManager * mEditorManager;
    QList<qReal::Id> mElements;
    QMap<QString, qReal::Id> mGestures;
};
