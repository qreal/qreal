#include "embeddedLinker.h"

#include <math.h>
#include <QtWidgets/QStyle>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include "editor/edgeElement.h"
#include "editor/nodeElement.h"

#include "editor/editorViewScene.h"
#include "editor/private/reshapeEdgeCommand.h"
#include "mainWindow/mainWindow.h"

using namespace qReal;

EmbeddedLinker::EmbeddedLinker()
		: mEdge(NULL)
		, mMaster(NULL)
		, mColor(Qt::blue)
		, mPressed(false)
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
}

EmbeddedLinker::~EmbeddedLinker()
{
}

NodeElement* EmbeddedLinker::master() const
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
	// TODO: It is not Label, it is simply some text on a scene. Refactor this.
	// Temporarily commented out.
//	EditorManagerInterface const &editorManagerInterface
//			= dynamic_cast<EditorViewScene *>(scene())->mainWindow()->editorManager();

//	QString edgeTypeFriendly = editorManagerInterface.friendlyName(Id::loadFromString("qrm:/"+ mMaster->id().editor()
//			+ "/" + mMaster->id().diagram() + "/" + mEdgeType.element()));

//	float textWidth = edgeTypeFriendly.size() * 10;
//	float rectWidth = mMaster->boundingRect().right() - mMaster->boundingRect().left();
//	float rectHeight = mMaster->boundingRect().bottom() - mMaster->boundingRect().top();

//	int x = 0;
//	int y = 0;
//	if (scenePos().y() < mMaster->scenePos().y() + rectHeight/3)
//		y = -boundingRect().height() - 10;
//	else if (scenePos().y() > mMaster->scenePos().y() + 2*rectHeight/3)
//		y = +boundingRect().height() - 10;

//	if (scenePos().x() < mMaster->scenePos().x() + rectWidth/3)
//		x = -boundingRect().width() - textWidth + 20;
//	else if (scenePos().x() > mMaster->scenePos().x() + 2*rectWidth/3)
//		x = +boundingRect().width() - 10;

//	mTitle = new Label(static_cast<qreal>(x) / boundingRect().width()
//			, static_cast<qreal>(y) / boundingRect().height(), edgeTypeFriendly, 0);

//	mTitle->init(boundingRect());
//	mTitle->setTextWidth(textWidth);
//	mTitle->setParentItem(this);
}

void EmbeddedLinker::setEdgeType(const qReal::Id &edgeType)
{
	this->mEdgeType = edgeType;
	generateColor();
}

qReal::Id EmbeddedLinker::edgeType() const
{
	return mEdgeType;
}

bool EmbeddedLinker::isDirected() const
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
			mInitialClickPoint = event->scenePos();
			Id const edgeId = scene->createElement(type, event->scenePos(), true, &mCreateEdgeCommand, false);
			mCreateEdgeCommand->redo();
			mEdge = dynamic_cast<EdgeElement*>(scene->getElem(edgeId));
		}

		if (mEdge) {
			mMaster->setZValue(1);
			mEdge->setSrc(mMaster);
			mEdge->setDst(NULL);
			mEdge->highlight();
			mEdge->tuneForLinker();
		}
	}

	mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	hide();
	mMaster->setSelectionState(false);
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(mMaster->scene());

	if (!mPressed && scene && mEdge) {
		mEdge->hide();
		QPointF const &eScenePos = event->scenePos();
		NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(eScenePos, QTransform()));
		mEdge->show();
		int result = 0;

		commands::CreateElementCommand *createElementFromMenuCommand = NULL;
		if (!under) {
			result = scene->launchEdgeMenu(mEdge, mMaster, eScenePos, false, &createElementFromMenuCommand);
		} else {
			bool canBeConnected = false;
			foreach(PossibleEdge const &pEdge, mEdge->src()->getPossibleEdges()) {
				if (pEdge.first.second.element() == under->id().element()) {
					canBeConnected = true;
					break;
				} else {
					// pEdge.second.first is true, if edge can connect items in only one direction.
					if (!pEdge.second.first) {
						canBeConnected = (pEdge.first.first.element() == under->id().element());
						if (canBeConnected) {
							break;
						}
					}
				}
			}

			if (under->isContainer()) {
				result = scene->launchEdgeMenu(mEdge, mMaster, eScenePos
						, canBeConnected, &createElementFromMenuCommand);
			} else {
				if (!canBeConnected) {
					result = -1;
				}
			}
		}
		NodeElement *target = dynamic_cast<NodeElement*>(scene->lastCreatedFromLinker());

		if (result == -1) {
			mEdge = NULL;
		} else if ((result == 1) && target) {
			mEdge->setDst(target);
			target->storeGeometry();
		}

		if (result != -1) {
			QPointF const position = mMaster->closestPortPoint(mInitialClickPoint, mEdge->fromPortTypes());
			mEdge->setSrc(mMaster);
			mEdge->setPos(position);
			mEdge->placeStartTo(QPointF());
			mEdge->placeEndTo(mEdge->mapFromScene(event->scenePos()));
			mEdge->connectToPort();
			// This will restore edge state after undo/redo
			commands::ReshapeEdgeCommand *reshapeEdge = new commands::ReshapeEdgeCommand(mEdge);
			reshapeEdge->startTracking();
			mEdge->layOut();
			reshapeEdge->stopTracking();
			reshapeEdge->setUndoEnabled(false);
			if (createElementFromMenuCommand) {
				createElementFromMenuCommand->addPostAction(reshapeEdge);
				createElementFromMenuCommand->addPreAction(mCreateEdgeCommand);
			} else {
				Controller * const controller = mEdge->controller();
				mCreateEdgeCommand->undo();
				mCreateEdgeCommand->addPostAction(reshapeEdge);
				mCreateEdgeCommand->setNewPosition(position);
				controller->execute(mCreateEdgeCommand);
			}
		}
	}
	mPressed = false;
	mEdge = NULL;
}

