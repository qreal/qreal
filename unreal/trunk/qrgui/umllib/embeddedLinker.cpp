#include "embeddedLinker.h"
#include "uml_nodeelement.h"

#include <QtGui/QStyle>
#include <QtGui/QGraphicsItem>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>
#include <math.h>

#include "../view/editorviewscene.h"

using namespace UML;
using namespace qReal;

EmbeddedLinker::EmbeddedLinker()
{
	covered = false;
	master = NULL;
	mEdge = NULL;
	mRectangle = QRectF(-6,-6,12,12);
	mInnerRectangle = QRectF(-3,-3,6,6);
	setAcceptsHoverEvents(true);

        //assign edge, color
        color = Qt::blue;

	QObject::connect(this,SIGNAL(coveredChanged()),this,SLOT(changeShowState()));
}

EmbeddedLinker::~EmbeddedLinker()
{

}

void EmbeddedLinker::setMaster(NodeElement *element)
{
	master = element;
	setParentItem(element);
	QObject::connect(master->scene(),SIGNAL(selectionChanged()),this,SLOT(changeShowState()));
}

void EmbeddedLinker::setCovered(bool arg)
{
	covered = arg;
	emit coveredChanged();
}

void EmbeddedLinker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	Q_UNUSED(option);
	painter->save();

	QBrush brush;
        brush.setColor(color);
	brush.setStyle(Qt::SolidPattern);
	painter->setBrush(brush);
	painter->setOpacity(0.5);
        painter->setPen(color);

	painter->drawEllipse(mRectangle);
	painter->setOpacity(0.7);
	painter->drawEllipse(mInnerRectangle);

	painter->restore();
}

void EmbeddedLinker::takePosition(int index, int maxIndex)
{
    const float Pi = 3.141592;
    QRectF bounding = master->boundingRect();

    float top = bounding.topLeft().y();
    float left = bounding.topLeft().x();
    float right = bounding.bottomRight().x();
    float bottom = bounding.bottomRight().y();
    float height = bottom - top;
    float width = right - left;

    char quadrant = 0;
    float angle = 2*Pi*index/maxIndex;
    if (angle > Pi/2)
	quadrant++;
    if (angle > Pi)
	quadrant++;
    if (angle > 3*Pi/2)
	quadrant++;

    float minRadius = qMin(height,width)/1.35;
    float maxRadius = qMax(height,width)/1.35;
    float radius = (maxRadius - minRadius)*(Pi/2 - (angle - quadrant*Pi/2))*2/Pi + minRadius;

    float x = left + width/2 + radius*cos(angle + Pi/2);
    float y = bottom - height/2 - radius*sin(angle + Pi/2);

    this->setPos(x,y);
}

QRectF EmbeddedLinker::boundingRect() const
{
	return mRectangle;
}

void EmbeddedLinker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	EditorViewScene *scene = dynamic_cast<EditorViewScene*>(master->scene());

	if (scene != NULL)
	{
		const QString type = "qrm:/Kernel_metamodel/Kernel/Kernel_Association";
		Id *edgeId = scene->createElement(type, event->scenePos());
		mEdge = dynamic_cast<EdgeElement*>(scene->getElem(*edgeId));
		mEdge->placeStartTo(master->getPortPos(0));
		if (mEdge != NULL)
		{
			mEdge->setSelected(true);
			master->setSelected(false);
			mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));	//QCursor?
		}
		else
			qDebug() << "*edge == NULL";
	}
}

void EmbeddedLinker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	hide();
	EditorViewScene *scene = dynamic_cast<EditorViewScene*>(master->scene());
	if (scene != NULL)
	{
		mEdge->hide();
		NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(event->scenePos()));
		mEdge->show();
		if (under == NULL)
			if (scene->launchEdgeMenu(mEdge, event->scenePos()))
				mEdge = NULL;
	}

	if (mEdge != NULL)
		mEdge->connectToPort();
}

void EmbeddedLinker::changeShowState()
{
	qDebug() << "segmentation...";
	QList<QGraphicsItem*> graphicsItems;
	if (scene())
	    graphicsItems = scene()->selectedItems();
	qDebug() << "...fault";
	if ((!master) || (!scene()) || (!covered) ||
	    ((!graphicsItems.contains(master)) && (!graphicsItems.contains(mEdge))))
	{
		hide();
		return;
	}
	else if ((graphicsItems.contains(master)) && (graphicsItems.size() == 1) && covered)
		show();
}
