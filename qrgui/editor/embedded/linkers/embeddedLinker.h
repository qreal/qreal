#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrkernel/roles.h>

#include "editor/edgeElement.h"
#include "models/commands/createElementCommand.h"

namespace qReal {

class NodeElement;

class EmbeddedLinker : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	EmbeddedLinker();
	EmbeddedLinker(QGraphicsItem *parent);
	virtual ~EmbeddedLinker();

	void initTitle();
	void generateColor();

	bool isDirected() const;
	qReal::Id edgeType() const;
	NodeElement* master() const;

	void setDirected(const bool directed);
	void setMaster(NodeElement* const master);
	void setEdgeType(qReal::Id const & edgeType);

	void takePosition(int index, int maxIndex);

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w);

	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
	EdgeElement* mEdge;
	NodeElement* mMaster;
	Label* mTitle;

	float mSize;
	float mIndent;
	QColor mColor;
	QRectF mRectangle;
	QRectF mInnerRectangle;

	bool mDirected;
	bool mPressed;
	qReal::Id mEdgeType;

	QPointF mInitialClickPoint;
	commands::CreateElementCommand *mCreateEdgeCommand;
};

}
