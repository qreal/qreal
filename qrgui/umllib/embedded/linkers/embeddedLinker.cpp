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
#include "../../private/reshapeEdgeCommand.h"

using namespace qReal;

EmbeddedLinker::EmbeddedLinker()
		: mEdge(NULL)
		, mMaster(NULL)
		, mColor(Qt::blue)
		, mPressed(false)
		, mTimeOfUpdate(0)
		, mTimer(new QTimer(this))
{
	mSize = SettingsManager::value("EmbeddedLinkerSize").toFloat();
	if (mSize > 10) {
		mSize *= 0.75;
	}
	mIndent = SettingsManager::value("EmbeddedLinkerIndent").toFloat();
	mIndent *= 0.8;
	if (mIndent > 17) {
		mIndent *= 0.7;
	}
	mRectangle = QRectF(-mSize, -mSize, mSize * 2, mSize * 2);
	mInnerRectangle = QRectF(-mSize / 2, -mSize / 2, mSize, mSize);
	setZValue(300);
	setFlag(ItemStacksBehindParent, false);

	setAcceptHoverEvents(true);

	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateMasterEdges()));
}

EmbeddedLinker::~EmbeddedLinker()
{
}

NodeElement* EmbeddedLinker::getMaster()
{
	return mMaster;
}

void EmbeddedLinker::setMaster(NodeElement *element)
{
	mMaster = element;
	setParentItem(element);
}

void EmbeddedLinker::generateColor()
{
	int result = 0;
	mColor = QColor(result % 192 + 64, result % 128 + 128, result % 64 + 192).darker(0);
}
void EmbeddedLinker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	Q_UNUSED(option);
	painter->save();

	QBrush brush;
	brush.setColor(mColor);
	brush.setStyle(Qt::SolidPattern);
	painter->setBrush(brush);
	painter->setOpacity(0.75);
	painter->setPen(mColor);

	mSize = SettingsManager::value("EmbeddedLinkerSize").toFloat();
	if (mSize > 10) {
		mSize *= 0.75;
	}
	mRectangle = QRectF(-mSize, -mSize, mSize * 2, mSize * 2);
	mInnerRectangle = QRectF(-mSize / 2, -mSize / 2, mSize, mSize);

	painter->drawEllipse(mRectangle);
	painter->setOpacity(0.9);
	painter->drawEllipse(mInnerRectangle);

	painter->restore();
}

void EmbeddedLinker::setDirected(const bool directed)
{
	this->mDirected = directed;
}

void EmbeddedLinker::initTitle()
{
	EditorManagerInterface const &editorManagerInterface = dynamic_cast<EditorViewScene*>(scene())->mainWindow()->editorManager();
	QString edgeTypeFriendly = editorManagerInterface.friendlyName(Id::loadFromString("qrm:/"+ mMaster->id().editor() + "/" + mMaster->id().diagram() + "/" + mEdgeType.element()));

	float textWidth = edgeTypeFriendly.size()*10;
	float rectWidth = mMaster->boundingRect().right() - mMaster->boundingRect().left();
	float rectHeight = mMaster->boundingRect().bottom() - mMaster->boundingRect().top();

	int x = 0;
	int y = 0;
	if (scenePos().y() < mMaster->scenePos().y() + rectHeight/3)
		y = -boundingRect().height() - 10;
	else if (scenePos().y() > mMaster->scenePos().y() + 2*rectHeight/3)
		y = +boundingRect().height() - 10;

	if (scenePos().x() < mMaster->scenePos().x() + rectWidth/3)
		x = -boundingRect().width() - textWidth + 20;
	else if (scenePos().x() > mMaster->scenePos().x() + 2*rectWidth/3)
		x = +boundingRect().width() - 10;

	mTitle = new ElementTitle(static_cast<qreal>(x) / boundingRect().width(), static_cast<qreal>(y) / boundingRect().height(), edgeTypeFriendly);
	mTitle->init(boundingRect());
	mTitle->setTextWidth(textWidth);
	mTitle->setParentItem(this);
}

void EmbeddedLinker::setEdgeType(const qReal::Id &edgeType)
{
	this->mEdgeType = edgeType;
	generateColor();
}

qReal::Id EmbeddedLinker::getEdgeType()
{
	return mEdgeType;
}

bool EmbeddedLinker::isDirected()
{
	return mDirected;
}

void EmbeddedLinker::takePosition(int index, int maxIndex)
{
	qreal const pi = 3.141592;
	QRectF const bounding = mMaster->boundingRect();

	qreal const top = bounding.topLeft().y();
	qreal const left = bounding.topLeft().x();
	qreal const right = bounding.bottomRight().x();
	qreal const bottom = bounding.bottomRight().y();
	qreal const height = bottom - top;
	qreal const width = right - left;

	qreal const angle = 2 * pi * index / maxIndex;

	int rW = width;
	int rH = height;
	if (rW < 150) {
		rW *= 1.5;
	} else {
		rW += 5;
	}
	if (rH < 150) {
		rH *= 1.5;
	} else {
		rH += 5;
	}

	// TODO: customize start angle
	qreal const px = left + width / 2 + rW * cos(angle) / 2;
	qreal const py = bottom - height / 2 + rH * sin(angle) / 2;

	// if linker covers master node:

	qreal min = py - top;
	if (min > bottom - py) {
		min = bottom - py;
	}
	if (min > px - left) {
		min = px - left;
	}
	if (min > right - px) {
		min = right - px;
	}

	qreal fx;
	qreal fy;
	mIndent = SettingsManager::value("EmbeddedLinkerIndent").toFloat();
	mIndent *= 0.8;
	if (mIndent > 17) {
		mIndent *= 0.7;
	}

	//obviously, top != left != right != bottom
	if ((bottom - py == min) || (py - top == min)) {
		fx = px;
		fy = bottom - py == min ? bottom + mIndent : top - mIndent;
	} else {
		fx = right - px == min ? right + mIndent : left - mIndent;
		fy = py;
	}

	setPos(fx, fy);
}

QRectF EmbeddedLinker::boundingRect() const {
	return mRectangle;
}

void EmbeddedLinker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
	mPressed = true;
	if (event->button() == Qt::LeftButton) {
		mEdge = NULL;
	}
}

void EmbeddedLinker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	mTimer->start(400);

	if (mPressed) {
		mPressed = false;
		EditorViewScene *scene = dynamic_cast<EditorViewScene*>(mMaster->scene());

		if (!scene) {
			return;
		}
		QString const type = "qrm:/" + mMaster->id().editor() + "/" +
							 mMaster->id().diagram() + "/" + mEdgeType.element();
		if (scene->mainWindow()->editorManager().hasElement(Id::loadFromString(type))) {
			mMaster->setConnectingState(true);
			// FIXME: I am raw. return strange pos() and inside me a small trash
			Id edgeId = scene->createElement(type, event->scenePos(), true, &mCreateEdgeCommand);
			mEdge = dynamic_cast<EdgeElement*>(scene->getElem(edgeId));
		}

		if (mEdge) {
			mMaster->setZValue(1);
			mEdge->setSrc(mMaster);
			mEdge->setDst(NULL);
			mEdge->highlight();
			mEdge->tuneForLinker();
			mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
			mMaster->arrangeLinks();
			mMaster->adjustLinks();
		}
	}

	if (mEdge) {
		if (mTimeOfUpdate == 14) {
			mTimeOfUpdate = 0;
			mEdge->adjustNeighborLinks();
			mEdge->arrangeSrcAndDst();
		} else {
			mTimeOfUpdate++;
		}
		mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
	}
}

void EmbeddedLinker::updateMasterEdges()
{
	mTimer->stop();
	mTimeOfUpdate = 0;

	if (mEdge) {
		mEdge->adjustNeighborLinks();
		mEdge->arrangeSrcAndDst();
	}
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	updateMasterEdges();
	hide();
	mMaster->selectionState(false);
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(mMaster->scene());

	if (!mPressed && scene && mEdge) {
		mEdge->hide();
		QPointF const &eScenePos = event->scenePos();
		NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(eScenePos, QTransform()));
		mEdge->show();
		int result = 0;

		commands::CreateElementCommand *createElementFromMenuCommand = NULL;
		if (!under) {
			result = scene->launchEdgeMenu(mEdge, mMaster, eScenePos, &createElementFromMenuCommand);
			NodeElement *target = dynamic_cast<NodeElement*>(scene->getLastCreated());
			if (result == -1) {
				mEdge = NULL;
			} else if ((result == 1) && target) {
				mEdge->setDst(target);
				target->storeGeometry();
			}
		}
		if (result != -1) {
			mEdge->connectToPort();
			mEdge->adjustNeighborLinks();
			// This will restore edge state after undo/redo
			commands::ReshapeEdgeCommand *reshapeEdge = new commands::ReshapeEdgeCommand(mEdge);
			reshapeEdge->startTracking();
			reshapeEdge->stopTracking();
			reshapeEdge->setUndoEnabled(false);
			if (createElementFromMenuCommand) {
				createElementFromMenuCommand->addPostAction(reshapeEdge);
				mCreateEdgeCommand->addPostAction(createElementFromMenuCommand);
			} else {
				mCreateEdgeCommand->addPostAction(reshapeEdge);
			}
		}
	}
	mPressed = false;
	mEdge = NULL;
}

