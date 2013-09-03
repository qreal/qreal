#pragma once

#include <qrkernel/ids.h>

#include "view/gestures/keyManagerInterface.h"
#include "view/gestures/keyManager.h"
#include "pluginManager/editorManagerInterface.h"
#include "mainwindow/gesturesPainterInterface.h"
#include "view/gestures/abstractRecognizer.h"
#include <QtCore/QPointF>
#include <QtCore/QPoint>
#include <QtCore/QLineF>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QMap>

namespace qReal {
namespace gestures {

class MouseMovementManager
{
public:
	MouseMovementManager(QList<qReal::Id> elements, qReal::EditorManagerInterface *editorManagerInterface
			, GesturesPainterInterface *gesturesPaintManager);

	~MouseMovementManager();

	void setElements(QList<qReal::Id> const &elements);
	void mousePress(QPointF const &point);
	void mouseMove(QPointF const &point);
	void clear();
	void setGesturesPainter(GesturesPainterInterface *gesturesPainter);
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
	IKeyManager *mKeyManager;
	KeyManager mKeyStringManager;
	qReal::EditorManagerInterface * mEditorManagerInterface;
	QList<qReal::Id> mElements;
	QPointF mCentre;
	GesturesPainterInterface *mGesturesPaintMan;
	GesturesManager *mGesturesManager;
};

}
}
