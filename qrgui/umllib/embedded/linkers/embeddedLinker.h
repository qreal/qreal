#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "../../../../qrkernel/roles.h"
#include "../../edgeElement.h"
#include "../../../controller/commands/createElementCommand.h"

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

	bool isDirected();
	qReal::Id getEdgeType();
	NodeElement* getMaster();

	void setDirected(const bool directed);
	void setMaster(NodeElement* const master);
	void setEdgeType(qReal::Id const & edgeType);

	void takePosition(int index, int maxIndex);

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w);

	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

private slots:
	void updateMasterEdges();

private:
	EdgeElement* mEdge;
	NodeElement* mMaster;
	ElementTitle* mTitle;

	float mSize;
	float mIndent;
	QColor mColor;
	QRectF mRectangle;
	QRectF mInnerRectangle;

	bool mDirected;
	bool mPressed;
	qReal::Id mEdgeType;

	int mTimeOfUpdate;
	QTimer *mTimer;
	commands::CreateElementCommand *mCreateEdgeCommand;
};
