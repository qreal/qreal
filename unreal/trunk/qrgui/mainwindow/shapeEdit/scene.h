#pragma once

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsView>
#include <QtCore/QList>

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

signals:
	void noSelectedItems();
	void existSelectedItems(QPen const &penItem, QBrush const &brushItem);

private slots:
	void drawLine(bool checked);
	void drawEllipse(bool checked);
	void drawCurve(bool checked);
	void drawRectangle(bool checked);
	void addText(bool checked);
	void addDynamicText(bool checked);
	void clearScene();
	void addPointPort(bool checked);
	void addLinePort(bool checked);
	void addStylus(bool checked);
	void addNone(bool checked);
	void deleteItem();
	void changePenStyle(const QString &text);
	void changePenWidth(int width);
	void changePenColor(const QString &text);
	void changeBrushStyle(const QString &text);
	void changeBrushColor(const QString &text);
	void changePalette();

private:
	enum ItemTypes {
		none,
		line,
		ellipse,
		rectangle,
		text,
		dynamicText,
		pointPort,
		linePort,
		stylus,
		curve
	};
	QGraphicsView *mView;
	ItemTypes mItemType;
	bool mWaitMove;
	int mCount;
	Line *mLine;
	Ellipse *mEllipse;
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
	QList<QGraphicsItem *> mListSelectedItems;

	QString convertPenToString(QPen const &pen);
	QString convertBrushToString(QBrush const &brush);
	void setPenBrushItems(QPen const &pen, QBrush const &brush);
	void setEmptyPenBrushItems();
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
	void setZValueItems(int index);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};
