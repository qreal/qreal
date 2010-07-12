#pragma once
#include "IKeyManager.h"
#include "keymanager.h"
#include "../../kernel/ids.h"
#include "../../editorManager/editorManager.h"
#include <QPointF>
#include <QPoint>
#include <QLineF>
#include <QList>
#include <QString>
#include <QMap>

class MouseMovementManager
{
public:
    MouseMovementManager(QList<qReal::Id> elements, qReal::EditorManager * editorManager);
    void setElements(QList<qReal::Id> const & elements);
    void addPoint(QPointF const & point);
    void clear();
    qReal::Id getObject();
    static QList<QPoint> stringToPath(QString const &str);
    QPointF pos();
    QLineF newLine();

private:
    static QPoint parsePoint(QString const &str);
    void createMap();
    QList<QPoint> mPath;
    IKeyManager * mKeyManager;
    KeyManager mKeyStringManager;
    qReal::EditorManager * mEditorManager;
    QList<qReal::Id> mElements;
    QMap<QString, qReal::Id> mGestures;
    QPointF mCentre;
};
