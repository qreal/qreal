#pragma once
#include "ikeymanager.h"
#include "keymanager.h"
#include "../../kernel/ids.h"
#include "../../editorManager/editorManager.h"
#include "../../mainwindow/igesturespainter.h"
#include <QPointF>
#include <QPoint>
#include <QLineF>
#include <QList>
#include <QString>
#include <QMap>


//todo:: унаследовать  от QObject
class MouseMovementManager
{
public:
    MouseMovementManager(QList<qReal::Id> elements,
                         qReal::EditorManager * editorManager,
                         IGesturesPainter * gesturesPaintManager);
    void setElements(QList<qReal::Id> const & elements);
    void addPoint(QPointF const & point);
    void clear();
    void setGesturesPainter(IGesturesPainter * gesturesPainter);
    qReal::Id getObject();
    static QList<QPoint> stringToPath(QString const &str);
    QPointF pos();
    QLineF newLine();
    void printElements();
    void drawIdealPath();

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
    IGesturesPainter * mGesturesPaintMan;
};
