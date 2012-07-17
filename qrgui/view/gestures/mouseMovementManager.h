#pragma once

#include "keyManagerInterface.h"
#include "keyManager.h"
#include "../../../qrkernel/ids.h"
#include "../../pluginManager/editorManagerInterface.h"
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
						 qReal::EditorManagerInterface * editorManagerInter,
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
	bool isEdgeCandidate();
	bool pathIsEmpty();

private:
	static QPoint parsePoint(QString const &str);
	void recountCentre();
	PathVector mPath;
	IKeyManager * mKeyManager;
	KeyManager mKeyStringManager;
	qReal::EditorManagerInterface * mEditorManagerInter;
	QList<qReal::Id> mElements;
	QPointF mCentre;
	GesturesPainterInterface * mGesturesPaintMan;
	GesturesManager * mGesturesManager;
};
