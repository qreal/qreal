#pragma once
#include "IKeyManager.h"
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


class MouseMovementManager
{
public:
        MouseMovementManager(QList<qReal::NewType> elements,
						 qReal::EditorManager * editorManager,
						 IGesturesPainter * gesturesPaintManager);
        void setElements(QList<qReal::NewType> const & elements);
	void addPoint(QPointF const & point);
	void clear();
	void setGesturesPainter(IGesturesPainter * gesturesPainter);
        qReal::NewType getObject();
	static QList<QPoint> stringToPath(QString const &str);
	QPointF pos();
	QPointF firstPoint();
	QPointF lastPoint();
	QLineF newLine();
	void printElements();
	void drawIdealPath();
	bool wasMoving();

private:
	static QPoint parsePoint(QString const &str);
	void createMap();
	QList<QPoint> mPath;
	IKeyManager * mKeyManager;
	KeyManager mKeyStringManager;
	qReal::EditorManager * mEditorManager;
        QList<qReal::NewType> mElements;
        QMap<QString, qReal::NewType> mGestures;
	QPointF mCentre;
	IGesturesPainter * mGesturesPaintMan;
};
