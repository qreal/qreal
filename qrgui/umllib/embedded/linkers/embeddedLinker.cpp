#include "../../edgeElement.h"
#include "../../nodeElement.h"
#include "embeddedLinker.h"

#include <math.h>

#include <QDebug>
#include <QStyle>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

#include "../../../view/editorViewScene.h"
#include "../../../mainwindow/mainWindow.h"

using namespace qReal;

EmbeddedLinker::EmbeddedLinker()
{
	size = SettingsManager::value("EmbeddedLinkerSize", 6).toFloat();
	indent = SettingsManager::value("EmbeddedLinkerIndent", 5).toFloat();

	mEdge = NULL;
	master = NULL;
	color = Qt::blue;
	mRectangle = QRectF(-size,-size,size*2,size*2);
	mInnerRectangle = QRectF(-size/2,-size/2,size,size);
	setAcceptsHoverEvents(true);
	color = Qt::blue;
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
}

void EmbeddedLinker::generateColor()
{
	int result = 0;
	//	QChar *data = edgeType.element().data();
	//	while (!data->isNull()) {
	//		result += data->unicode();
	//		++data;
	//	}
	//	result *= 666;
	color = QColor(result % 192 + 64, result % 128 + 128, result % 64 + 192).darker(0);
}

//void EmbeddedLinker::setColor(QColor arg)
//{
//	color = arg;
//}

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

void EmbeddedLinker::setDirected(const bool directed)
{
	this->directed = directed;
}

void EmbeddedLinker::initTitle()
{
	EditorManager* editorManager = dynamic_cast<EditorViewScene*>(scene())->mainWindow()->manager();
	QString edgeTypeFriendly = editorManager->friendlyName(Id::loadFromString("qrm:/"+master->id().editor()+"/"+master->id().diagram()+"/"+edgeType.element()));

	float textWidth = edgeTypeFriendly.size()*10;
	float rectWidth = master->boundingRect().right() - master->boundingRect().left();
	float rectHeight = master->boundingRect().bottom() - master->boundingRect().top();

	int x = 0;
	int y = 0;
	if (scenePos().y() < master->scenePos().y() + rectHeight/3)
		y = -boundingRect().height() - 10;
	else if (scenePos().y() > master->scenePos().y() + 2*rectHeight/3)
		y = +boundingRect().height() - 10;

	if (scenePos().x() < master->scenePos().x() + rectWidth/3)
		x = -boundingRect().width() - textWidth + 20;
	else if (scenePos().x() > master->scenePos().x() + 2*rectWidth/3)
		x = +boundingRect().width() - 10;

	title = new ElementTitle(static_cast<qreal>(x) / boundingRect().width(), static_cast<qreal>(y) / boundingRect().height(), edgeTypeFriendly);
	title->init(boundingRect());
	title->setTextWidth(textWidth);
	title->setParentItem(this);
}

void EmbeddedLinker::setEdgeType(const qReal::Id &edgeType)
{
	this->edgeType = edgeType;
	generateColor();
}

qReal::Id EmbeddedLinker::getEdgeType()
{
	return edgeType;
}

bool EmbeddedLinker::isDirected()
{
	return directed;
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

	setPos(fx,fy);

	//useful for debug:
	//scene()->addPolygon(master->mapToScene(master->boundingRect().left(),master->boundingRect().top(),
	//									master->boundingRect().width(),master->boundingRect().height()));
}

QRectF EmbeddedLinker::boundingRect() const {
	return mRectangle;
}

void EmbeddedLinker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
	mPressed = true;
}

void EmbeddedLinker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mPressed) {
		mPressed = false;
		EditorViewScene *scene = dynamic_cast<EditorViewScene*>(master->scene());

		if (!scene) {
			return;
		}
		const QString type = "qrm:/" + master->id().editor() + "/" +
				master->id().diagram() + "/" + edgeType.element();
		if (scene->mainWindow()->manager()->hasElement(Id::loadFromString(type))) {
			master->setConnectingState(true);
			Id *edgeId = scene->createElement(type, event->scenePos());
			mEdge = dynamic_cast<EdgeElement*>(scene->getElem(*edgeId));
		}

		if (mEdge){
			QPointF point = mapToItem(master, event->pos());
			mEdge->placeStartTo(mEdge->mapFromItem(master, master->getNearestPort(point)));
			mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
		}
	}

	if (mEdge != NULL) {
		mEdge->arrangeSrcAndDst();
		mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
	}
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	hide();
	master->selectionState(false);
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(master->scene());

	if (!mPressed && scene && mEdge) {
		mEdge->hide();
		QPointF const &eScenePos = event->scenePos();
		NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(eScenePos));
		mEdge->show();
		int result = 0;

		if (!under) {
			result = scene->launchEdgeMenu(mEdge, master, eScenePos);
			NodeElement *target = dynamic_cast<NodeElement*>(scene->getLastCreated());
			if (result == -1) {
				mEdge = NULL;
			} else if ((result == +1) && target) {
				QPointF const &posRelativeToTheTarget = target->mapFromScene(eScenePos);
				mEdge->placeEndTo(mapFromItem(target, target->getNearestPort(posRelativeToTheTarget)));
				mEdge->connectToPort();	//it provokes to move target somehow, so it needs to place edge end and connect to port again
				mEdge->placeEndTo(mapFromItem(target, target->getNearestPort(posRelativeToTheTarget)));
				mEdge->adjustLink();
			}
		}
		if (result != -1) {
			mEdge->connectToPort();
		}
	}
}

