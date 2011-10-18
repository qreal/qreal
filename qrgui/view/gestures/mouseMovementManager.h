#pragma once
#include "keyManagerInterface.h"
#include "keyManager.h"
#include "../../../qrkernel/ids.h"
#include "../../pluginManager/editorManager.h"
#include "../../mainwindow/gesturesPainterInterface.h"
#include "abstractRecognizer.h"
#include <QPointF>
#include <QPoint>
#include <QLineF>
#include <QList>
#include <QString>
#include <QMap>


class MouseMovementManager
{
public:
    MouseMovementManager(QList<qReal::Id> elements,
                         qReal::EditorManager * editorManager,
                         GesturesPainterInterface * gesturesPaintManager);
    void setElements(QList<qReal::Id> const & elements);
    void mousePress(QPointF const & point);
    void mouseMove(QPointF const & point);
    void clear();
    void setGesturesPainter(GesturesPainterInterface * gesturesPainter);
    qReal::Id getObject();
    static PathVector stringToPath(QString const &str);
    QPointF pos();
    QPointF firstPoint();
    QPointF lastPoint();
    QLineF newLine();
    void printElements();
    void drawIdealPath();
    bool wasMoving();

private:
    static QPoint parsePoint(QString const &str);
    void recountCentre();
    PathVector mPath;
    IKeyManager * mKeyManager;
    KeyManager mKeyStringManager;
    qReal::EditorManager * mEditorManager;
    QList<qReal::Id> mElements;
    QPointF mCentre;
    GesturesPainterInterface * mGesturesPaintMan;
    GesturesManager * mGesturesManager;
};
