#pragma once
#include "keyManagerInterface.h"
#include "keyManager.h"
#include "../../kernel/ids.h"
#include "../../editorManager/editorManager.h"
#include "../../mainwindow/gesturesPainterInterface.h"
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
	void addPoint(QPointF const & point);
	void clear();
	void setGesturesPainter(GesturesPainterInterface * gesturesPainter);
	qReal::Id getObject();
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
	QList<qReal::Id> mElements;
	QMap<QString, qReal::Id> mGestures;
	QPointF mCentre;
	GesturesPainterInterface * mGesturesPaintMan;
};
