#include "embeddedLinker.h"
#include "uml_nodeelement.h"
#include "uml_edgeelement.h"

#include <math.h>
#include <QtGui/QStyle>
#include <QtGui/QGraphicsItem>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>
#include <QSettings>

#include "../view/editorviewscene.h"
#include "../mainwindow/mainwindow.h"

using namespace UML;
using namespace qReal;

EmbeddedLinker::EmbeddedLinker()
{
	QSettings settings("SPbSU", "QReal");
	size = settings.value("EmbeddedLinkerSize", 6).toFloat();
	indent = settings.value("EmbeddedLinkerIndent", 5).toFloat();

	covered = false;
	master = NULL;
	mEdge = NULL;
	mRectangle = QRectF(-size,-size,size*2,size*2);
	mInnerRectangle = QRectF(-size/2,-size/2,size,size);
	setAcceptsHoverEvents(true);
	color = Qt::blue;

	QObject::connect(this,SIGNAL(coveredChanged()),this,SLOT(changeShowState()));
}

EmbeddedLinker::~EmbeddedLinker()
{

}

NodeElement* EmbeddedLinker::getMaster()
{
	return master;
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

void EmbeddedLinker::generateColor()
{
	int result = 0;
	QChar *data = edgeType.element().data();
	while (!data->isNull()) {
		result += data->unicode();
		++data;
	}
	result *= 666;
	color =QColor(result%192+64,result%128+128,result%64+192).darker(0);
}

void EmbeddedLinker::setColor(QColor arg)
{
		color = arg;
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

void EmbeddedLinker::setDirected(bool arg)
{
	directed = arg;
}

void EmbeddedLinker::initTitle()
{
	EditorManager* editorManager = dynamic_cast<EditorViewScene*>(scene())->mainWindow()->manager();
	QString edgeTypeFriendly = editorManager->friendlyName(Id::loadFromString("qrm:/"+master->uuid().editor()+"/"+master->uuid().diagram()+"/"+edgeType.element()));

	float textWidth = edgeTypeFriendly.size()*10;
	float rectWidth = master->boundingRect().right() - master->boundingRect().left();
	float rectHeight = master->boundingRect().bottom() - master->boundingRect().top();

	int x = 0;
	int y = 0;
	if (this->scenePos().y() < master->scenePos().y() + rectHeight/3)
		y = -boundingRect().height() - 10;
	else if (this->scenePos().y() > master->scenePos().y() + 2*rectHeight/3)
		y = +boundingRect().height() - 10;

	if (this->scenePos().x() < master->scenePos().x() + rectWidth/3)
		x = -boundingRect().width() - textWidth + 20;
	else if (this->scenePos().x() > master->scenePos().x() + 2*rectWidth/3)
		x = +boundingRect().width() - 10;

	title = new ElementTitle(x,y,edgeTypeFriendly);
	title->setTextWidth(textWidth);
	title->setParentItem(this);
}

void EmbeddedLinker::setEdgeType(const qReal::Id &arg)
{
	edgeType = arg;
	generateColor();
}

bool EmbeddedLinker::isDirected()
{
	return directed;
}

qReal::Id EmbeddedLinker::getEdgeType()
{
	return edgeType;
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

	float angle = 2*Pi*index/maxIndex;

	int rW = width;
	int rH = height;
	if (rW < 150)
		rW *= 1.5;
	else
		rW += 5;
	if (rH < 150)
		rH *= 1.5;
	else
		rH += 5;

	float px = left + width/2 + rW*cos(angle - Pi/2)/2;
	float py = bottom - height/2 + rH*sin(angle - Pi/2)/2;

	//if linker covers master node:

	float min = py - top;
	if (min > bottom - py)
		min = bottom - py;
	if (min > px - left)
		min = px - left;
	if (min > right - px)
		min = right - px;

	float fx;
	float fy;

	//obviously, top != left != right != bottom
	if ((bottom - py == min) || (py - top == min))
	{
		fx = px;
		if (bottom - py == min)
			fy = bottom + indent;
		else
			fy = top - indent;
	}
	else
	{
		fy = py;
		if (right - px == min)
			fx = right + indent;
		else
			fx = left - indent;
	}

	this->setPos(fx,fy);

	//useful for debug:
	//scene()->addPolygon(master->mapToScene(master->boundingRect().left(),master->boundingRect().top(),
	//									master->boundingRect().width(),master->boundingRect().height()));
}

QRectF EmbeddedLinker::boundingRect() const
{
	return mRectangle;
}

void EmbeddedLinker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	EditorViewScene *scene = dynamic_cast<EditorViewScene*>(master->scene());

	if (scene)
	{
		const QString type = "qrm:/" + master->uuid().editor() + "/" +
							 master->uuid().diagram() + "/" + edgeType.element();
		if (scene->mainWindow()->manager()->hasElement(Id::loadFromString(type)))
		{
			master->setConnectingState(true);
			Id *edgeId = scene->createElement(type, event->scenePos());
			mEdge = dynamic_cast<EdgeElement*>(scene->getElem(*edgeId));
		}
		if (mEdge)
		{
			master->setSelected(false);
			QPointF point = mapToItem(master,event->pos());
			mEdge->placeStartTo(mEdge->mapFromItem(master,master->getNearestPort(point)));
			mEdge->placeEndTo(mapToItem(mEdge,event->pos()));
		}
	}
}

void EmbeddedLinker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mEdge != NULL)
		mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	hide();
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(master->scene());
	if ((scene) && (mEdge))
	{
		mEdge->hide();
		NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(event->scenePos()));
		mEdge->show();
		int result = 0;
		UML::NodeElement* target;

		if (!under) {
			result = scene->launchEdgeMenu(mEdge, master, event->scenePos());
			if (result == -1)
				mEdge = NULL;
			else if ((result == +1) && (scene->getLastCreated()))
			{
				target = dynamic_cast<UML::NodeElement*>(scene->getLastCreated());
				if (target) {
					mEdge->placeEndTo(mapFromItem(target,target->getNearestPort(target->pos())));
					mEdge->connectToPort();	//it provokes to move target somehow, so it needs to place edge end and connect to port again
					mEdge->placeEndTo(mapFromItem(target,target->getNearestPort(target->pos())));
				}
			}
		}
		if (result != -1)
			mEdge->connectToPort();
	}
}

void EmbeddedLinker::changeShowState()
{
	QList<QGraphicsItem*> graphicsItems;
	if (scene())
		graphicsItems = scene()->selectedItems();
	if ((!master) || (!scene()) || (!covered) ||
		((!graphicsItems.contains(master)) && (!graphicsItems.contains(mEdge))))
	{
		hide();
		return;
	}
	else if ((graphicsItems.contains(master)) && (graphicsItems.size() == 1) && covered)
		show();
}
