#pragma once

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "item.h"
#include "arch.h"
#include "line.h"
#include "ellipse.h"
#include "rectangle.h"
#include "text.h"
#include "../umllib/elementTitle.h"

using namespace UML;

class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	Scene(QObject *parent = 0);
	QGraphicsRectItem *mEmptyRect;
private slots:
	void drawLine();
	void drawEllipse();
	void drawArc();
	void drawRectangle();
	void addText();
	void addDynamicText();
	void clearScene();
private:
	bool mWaitLine;
	bool mWaitMoveLine;
	bool mWaitEllipse;
	bool mWaitMoveEllipse;
	bool mWaitArch;
	bool mWaitRectangle;
	bool mWaitMoveRectangle;
	bool mWaitText;
	bool mWaitDynamicText;
	int mCount;
	Line *mLine;
	Ellipse *mEllipse;
	Arch *mArch;
	Rectangle *mRectangle;
	Text* mText;
	qreal mX1;
	qreal mX2;
	qreal mY1;
	qreal mY2;

	void setX1andY1(QGraphicsSceneMouseEvent *event);
	void setX2andY2(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);
	void reshapeRectangle(QGraphicsSceneMouseEvent *event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};
