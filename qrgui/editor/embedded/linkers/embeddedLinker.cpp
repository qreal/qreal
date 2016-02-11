/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "embeddedLinker.h"

#include <math.h>
#include <QtWidgets/QStyle>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include "editor/edgeElement.h"
#include "editor/nodeElement.h"

#include "editor/editorViewScene.h"
#include "editor/commands/reshapeEdgeCommand.h"

using namespace qReal;
using namespace qReal::commands;
using namespace qReal::gui::editor;

EmbeddedLinker::EmbeddedLinker()
		: mEdge(nullptr)
		, mMaster(nullptr)
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
//	const EditorManagerInterface &editorManagerInterface
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

void EmbeddedLinker::setEdgeType(const Id &edgeType)
{
	this->mEdgeType = edgeType;
	generateColor();
}

Id EmbeddedLinker::edgeType() const
{
	return mEdgeType;
}

bool EmbeddedLinker::isDirected() const
{
	return mDirected;
}

void EmbeddedLinker::takePosition(int index, int maxIndex)
{
	const qreal pi = 3.141592;
	const QRectF bounding = mMaster->boundingRect();

	const qreal top = bounding.topLeft().y();
	const qreal left = bounding.topLeft().x();
	const qreal right = bounding.bottomRight().x();
	const qreal bottom = bounding.bottomRight().y();
	const qreal height = bottom - top;
	const qreal width = right - left;

	const qreal angle = 2 * pi * index / maxIndex;

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
	const qreal px = left + width / 2 + rW * cos(angle) / 2;
	const qreal py = bottom - height / 2 + rH * sin(angle) / 2;

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
		mEdge = nullptr;
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

		const QString type = "qrm:/" + mMaster->id().editor() + "/" +
							 mMaster->id().diagram() + "/" + mEdgeType.element();
		if (scene->editorManager().hasElement(Id::loadFromString(type))) {
			mMaster->setConnectingState(true);
			mInitialClickPoint = event->scenePos();
			const Id edgeId = scene->createElement(type, event->scenePos(), &mCreateEdgeCommand, false);
			mCreateEdgeCommand->redo();
			mEdge = dynamic_cast<EdgeElement*>(scene->getElem(edgeId));
		}

		if (mEdge) {
			mMaster->setZValue(1);
			mEdge->setSrc(mMaster);
			mEdge->setDst(nullptr);
			mEdge->highlight();
			mEdge->tuneForLinker();
		}
	}

	mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	hide();
	mMaster->setConnectingState(false);
	mMaster->setSelected(false);
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(mMaster->scene());

	if (!mPressed && scene && mEdge) {
		mEdge->hide();
		const QPointF &eScenePos = event->scenePos();
		NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(eScenePos, QTransform()));
		mEdge->show();
		int result = 0;

		CreateElementsCommand *createElementFromMenuCommand = nullptr;
		if (!under) {
			result = scene->launchEdgeMenu(mEdge, mMaster, eScenePos, false, &createElementFromMenuCommand);
		} else {
			bool canBeConnected = false;
			for (const PossibleEdge &pEdge : mEdge->src()->getPossibleEdges()) {
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
			mEdge = nullptr;
		} else if ((result == 1) && target) {
			mEdge->setDst(target);
			target->storeGeometry();
		}

		if (result != -1) {
			const QPointF position = mMaster->closestPortPoint(mInitialClickPoint, mEdge->fromPortTypes());
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
	mEdge = nullptr;
}
