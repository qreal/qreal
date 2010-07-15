#pragma once

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsView>

#include "item.h"
#include "arch.h"
#include "line.h"
#include "ellipse.h"
#include "rectangle.h"
#include "text.h"
#include "pointPort.h"
#include "linePort.h"
#include "stylus.h"
#include "path.h"
#include "../umllib/elementTitle.h"

using namespace UML;

const int sizeEmrtyRectX = 690;
const int sizeEmrtyRectY = 590;

class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	Scene(QGraphicsView *view, QObject *parent = 0);
	QGraphicsRectItem *mEmptyRect;
	QPointF centerEmpty();

private slots:
	void drawLine();
	void drawEllipse();
	void drawArc();
	void drawRectangle();
	void addText();
	void addDynamicText();
	void clearScene();
	void addPointPort();
	void addLinePort();
	void addStylus();
	void deleteItem();
	void changePenStyle(const QString &text);
	void changePenWidth(int width);
	void changePenColor(const QString &text);
	void changeBrushStyle(const QString &text);
	void changeBrushColor(const QString &text);

private:
	enum ItemTypes {
		none,
		line,
		ellipse,
		arch,
		rectangle,
		text,
		dynamicText,
		pointPort,
		linePort,
		stylus
	};
	QGraphicsView *mView;
	ItemTypes mItemType;
	bool mWaitMove;
	int mCount;
	Line *mLine;
	Ellipse *mEllipse;
	Arch *mArch;
	Rectangle *mRectangle;
	Text *mText;
	PointPort *mPointPort;
	LinePort *mLinePort;
	Stylus *mStylus;
	Item *mGraphicsItem;
	qreal mX1;
	qreal mX2;
	qreal mY1;
	qreal mY2;
	QString mPenStyleItems;
	int mPenWidthItems;
	QString mPenColorItems;
	QString mBrushStyleItems;
	QString mBrushColorItems;

	void setX1andY1(QGraphicsSceneMouseEvent *event);
	void setX2andY2(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeLinePort(QGraphicsSceneMouseEvent *event);
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);
	void reshapeRectangle(QGraphicsSceneMouseEvent *event);
	void reshapeStylus(QGraphicsSceneMouseEvent *event);
	void reshapeItem(QGraphicsSceneMouseEvent *event);

	void removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem* item);
	void setMoveFlag(QGraphicsSceneMouseEvent *event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};
